#ifdef SSD1306OLED

#include "logo.h"
#include "ssd1306.h"
#include "config.h"
#include "i2c.h"
#include "print.h"

#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

#define i2cAddress 0x3C

#define DisplayHeight 32
#define DisplayWidth 128

#define FontHeight 8
#define FontWidth 6

#define MatrixRows (DisplayHeight / FontHeight)
#define MatrixCols (DisplayWidth / FontWidth)

bool dirty = true;

enum ssd1306_cmds {
  DisplayOff = 0xAE,
  DisplayOn = 0xAF,

  SetContrast = 0x81,
  DisplayAllOnResume = 0xA4,

  DisplayAllOn = 0xA5,
  NormalDisplay = 0xA6,
  InvertDisplay = 0xA7,
  SetDisplayOffset = 0xD3,
  SetComPins = 0xda,
  SetVComDetect = 0xdb,
  SetDisplayClockDiv = 0xD5,
  SetPreCharge = 0xd9,
  SetMultiPlex = 0xa8,
  SetLowColumn = 0x00,
  SetHighColumn = 0x10,
  SetStartLine = 0x40,

  SetMemoryMode = 0x20,
  ColumnAddr = 0x21,
  PageAddr = 0x22,

  ComScanInc = 0xc0,
  ComScanDec = 0xc8,
  SegRemap = 0xa0,
  SetChargePump = 0x8d,
  ExternalVcc = 0x01,
  SwitchCapVcc = 0x02,

  ActivateScroll = 0x2f,
  DeActivateScroll = 0x2e,
  SetVerticalScrollArea = 0xa3,
  RightHorizontalScroll = 0x26,
  LeftHorizontalScroll = 0x27,
  VerticalAndRightHorizontalScroll = 0x29,
  VerticalAndLeftHorizontalScroll = 0x2a,
};

// Write command sequence.
// Returns true on success.
static inline bool _send_cmd1(uint8_t cmd) {
  bool res = false;

  if (i2c_start_write(i2cAddress)) {
    xprintf("failed to start write to %d\n", i2cAddress);
    goto done;
  }

  if (i2c_master_write(0x0 /* command byte follows */)) {
    print("failed to write control byte\n");

    goto done;
  }

  if (i2c_master_write(cmd)) {
    xprintf("failed to write command %d\n", cmd);
    goto done;
  }
  res = true;
done:
  i2c_master_stop();
  return res;
}

// Write 2-byte command sequence.
// Returns true on success
static inline bool _send_cmd2(uint8_t cmd, uint8_t opr) {
  if (!_send_cmd1(cmd)) {
    return false;
  }
  return _send_cmd1(opr);
}

// void rotate(uint8_t a[8], int m, int n, uint8_t b[8]) {
//   uint8_t x, y, t;
//   x = (a[0]<<24)   | (a[m]<<16)   | (a[2*m]<<8) | a[3*m];
//   y = (a[4*m]<<24) | (a[5*m]<<16) | (a[6*m]<<8) | a[7*m];

//   t = (x ^ (x >> 7)) & 0x00AA00AA; x = x ^ t ^ (t << 7);
//   t = (y ^ (y >> 7)) & 0x00AA00AA; y = y ^ t ^ (t << 7);

//   t = (x ^ (x >>14)) & 0x0000CCCC; x = x ^ t ^ (t <<14);
//   t = (y ^ (y >>14)) & 0x0000CCCC; y = y ^ t ^ (t <<14);

//   t = (x & 0xF0F0F0F0) | ((y >> 4) & 0x0F0F0F0F);
//   y = ((x << 4) & 0xF0F0F0F0) | (y & 0x0F0F0F0F);
//   x = t;

//   b[0]   = x>>24; b[n]   = x>>16; b[2*n] = x>>8; b[3*n] = x;
//   b[4*n] = y>>24; b[5*n] = y>>16; b[6*n] = y>>8; b[7*n] = y;
// }

// Write 3-byte command sequence.
// Returns true on success
static inline bool _send_cmd3(uint8_t cmd, uint8_t opr1, uint8_t opr2) {
  if (!_send_cmd1(cmd)) {
    return false;
  }
  if (!_send_cmd1(opr1)) {
    return false;
  }
  return _send_cmd1(opr2);
}

#define send_cmd1(c) if (!_send_cmd1(c)) {goto done;}
#define send_cmd2(c,o) if (!_send_cmd2(c,o)) {goto done;}
#define send_cmd3(c,o1,o2) if (!_send_cmd3(c,o1,o2)) {goto done;}

void logo_render(void) {
  if (!dirty) {
    return;
  }

  // last_flush = timer_read();
  iota_gfx_on();

  send_cmd2(SetMemoryMode, 1);

  // Move to the home position
  send_cmd3(PageAddr, 0, MatrixRows - 1);
  send_cmd3(ColumnAddr, 0, DisplayWidth - 1);

  if (i2c_start_write(i2cAddress)) {
    goto done;
  }
  if (i2c_master_write(0x40)) {
    // Data mode
    goto done;
  }
  // static uint8_t invader[2][16] = {{0x0,0x4,0xfd,0xff,0x76,0xfc,0x88,0x88,0x0,0x4,0xfe,0xff,0x77,0xfd,0x89,0x4,},{0x0,0x1,0x5,0x7,0x3,0x1,0x0,0x0,0x0,0x1,0x3,0x7,0x7,0x5,0x4,0x1,},};
  static uint8_t rows = sizeof(swarrow) / sizeof(swarrow[0]);
  static uint8_t cols = sizeof(swarrow[0]) / sizeof(swarrow[0][0]);

  for (uint8_t row = 0; row < DisplayWidth-1; row++) {
    const uint8_t *s = (uint8_t *)swarrow + row % rows * sizeof(swarrow[0]);
    for (uint8_t col = 0; col < DisplayHeight / FontHeight; col++) {
      uint8_t colBits = pgm_read_byte(s+col % cols);
      i2c_master_write(colBits);
    }
  }

  send_cmd1(VerticalAndLeftHorizontalScroll);
  send_cmd3(0,0,0);
  send_cmd2(0x3,0x3f);
  send_cmd3(SetVerticalScrollArea, 0, DisplayHeight);
  // send_cmd1(RightHorizontalScroll);
  // send_cmd3(0,0,1);
  // send_cmd3(3,0,0xff);
  send_cmd1(ActivateScroll);

  dirty = false;

done:
  i2c_master_stop();
}

#endif
