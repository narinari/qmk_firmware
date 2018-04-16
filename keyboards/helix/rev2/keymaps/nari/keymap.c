#include "helix.h"
#include "bootloader.h"
#include "action_layer.h"
#include "eeconfig.h"
#include "LUFA/Drivers/Peripheral/TWI.h"
#ifdef AUDIO_ENABLE
  #include "audio.h"
  #define PC98_SOUND \
    E__NOTE(_C7 ),   \
    E__NOTE(_C6 ),
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
  #include "logo.h"
#endif

extern keymap_config_t keymap_config;

//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _COLEMAK 1
#define _DVORAK 2
#define _LOWER 3
#define _RAISE 4
#define _ADJUST 16

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  LOWER,
  RAISE,
  ADJUST,
  BACKLIT,
  RGBRST
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};


// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO
//Macros
#define M_SAMPLE M(KC_SAMPLEMACRO)

#if HELIX_ROWS == 5
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Del  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |             |   Y  |   U  |   I  |   O  |   P  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  '   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |   [  |   ]  |   N  |   M  |   ,  |   .  |   /  |Enter |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Adjust|Lower |Lower |Lower |Lower |Lower |Space |Space |Raise |Raise | Left | Down |  Up  |Right |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_QWERTY] = KEYMAP( \
    KC_ESC,  KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,                            KC_Y,      KC_U,      KC_I,    KC_O,    KC_P,    KC_MINS, \
    KC_LCTL, KC_A,      KC_S,      KC_D,      KC_F,      KC_G,                            KC_H,      KC_J,      KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
    KC_LSFT, KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,                            KC_N,      KC_M,      KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_ESC,  KC_BSPC,   KC_LCTL,   KC_LALT,   KC_LGUI,   KC_SPC,    KC_BSPC,   KC_TAB,    KC_ENT,    KC_RGUI,   KC_RALT, KC_RCTL, KC_TAB,  KC_ESC, \
    ADJUST,  TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(RAISE), TT(RAISE), TT(RAISE), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
    ),

  /* Colemak
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Del  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Tab  |   Q  |   W  |   F  |   P  |   G  |             |   J  |   L  |   U  |   Y  |   ;  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   R  |   S  |   T  |   D  |             |   H  |   N  |   E  |   I  |   O  |  '   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |   [  |   ]  |   K  |   M  |   ,  |   .  |   /  |Enter |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Adjust|Lower |Lower |Lower |Lower |Lower |Lower |Raise |Raise |Raise | Left | Down |  Up  |Right |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_COLEMAK] = KEYMAP( \
    KC_ESC,  KC_Q,      KC_W,      KC_F,      KC_P,      KC_G,                            KC_J,      KC_L,      KC_U,    KC_Y,    KC_SCLN, KC_MINS, \
    KC_LCTL, KC_A,      KC_R,      KC_S,      KC_T,      KC_D,                            KC_H,      KC_N,      KC_E,    KC_I,    KC_O,    KC_QUOT, \
    KC_LSFT, KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,                            KC_K,      KC_M,      KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_ESC,  KC_BSPC,   KC_LCTL,   KC_LALT,   KC_LGUI,   KC_SPC,    KC_BSPC,   KC_TAB,    KC_ENT,    KC_RGUI,   KC_RALT, KC_RCTL, KC_TAB,  KC_ESC, \
    ADJUST,  TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(RAISE), TT(RAISE), TT(RAISE), KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
    ),

  /* Dvorak
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Tab  |   '  |   ,  |   .  |   P  |   Y  |             |   F  |   G  |   C  |   R  |   L  | Del  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   U  |   I  |             |   D  |   H  |   T  |   N  |   S  |  /   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * | Shift|   ;  |   Q  |   J  |   K  |   X  |   [  |   ]  |   B  |   M  |   W  |   V  |   Z  |Enter |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Adjust|Lower |Lower |Lower |Lower |Lower |Raise |Raise |Raise |Raise |Raise |Raise |Raise |Raise |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_DVORAK] = KEYMAP( \
    KC_ESC,  KC_QUOT,   KC_COMM,   KC_DOT,    KC_P,      KC_Y,                            KC_F,      KC_G,      KC_C,    KC_R,    KC_L,   KC_DEL, \
    KC_LCTL, KC_A,      KC_O,      KC_E,      KC_U,      KC_I,                            KC_D,      KC_H,      KC_T,    KC_N,    KC_S,   KC_SLSH, \
    KC_LSFT, KC_SCLN,   KC_Q,      KC_J,      KC_K,      KC_X,                            KC_B,      KC_M,      KC_W,    KC_V,    KC_Z,   KC_ENT , \
    KC_ESC,  KC_BSPC,   KC_LCTL,   KC_LALT,   KC_LGUI,   KC_SPC,    KC_BSPC,   KC_TAB,    KC_ENT,    KC_RGUI,   KC_RALT, KC_RCTL, KC_TAB, KC_ESC, \
    ADJUST,  TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(LOWER), TT(RAISE), TT(RAISE), TT(RAISE), KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT \
    ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |   *  |   (  |   )  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |   *  |   (  |   )  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   _  |   +  |   {  |   }  |  |   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  (   |   )  |  F12 |      |      | Home | End  |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = KEYMAP( \
    _______, _______, KC_BTN1, KC_MS_U, KC_BTN2, _______,                   _______, _______, _______, _______, KC_END,  KC_HOME, \
    _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PGDN, KC_PGUP, \
    _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
    ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Bksp |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Del  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   -  |   =  |   [  |   ]  |  \   |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |  F7  |  F8  |  F9  |  F10 |  F11 |      |      |  F12 |      |      |PageDn|PageUp|      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = KEYMAP( \
    _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,                   KC_BSLS, KC_7,    KC_8,    KC_9,    KC_ASTR, _______, \
    _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,                    KC_EQL,  KC_4,    KC_5,    KC_6,    KC_PLUS, KC_PIPE, \
    _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,                   KC_AMPR, KC_1,    KC_2,    KC_3,    KC_MINS, _______, \
    _______, _______, _______, _______, _______, KC_BSPC, KC_DEL,  _______, _______, KC_0,    KC_COMM, KC_DOT,  KC_EQL,  _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
    ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | Reset|      |      |      |      |             |      |      |      |      |      |  Del |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |Aud on|Audoff| Mac  |             | Win  |Qwerty|Colemk|Dvorak|      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |RGB ON| HUE+ | SAT+ | VAL+ |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      | MODE | HUE- | SAT- | VAL- |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  KEYMAP( \
    RESET,   RGBRST,  RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI,          RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, BACKLIT, KC_DEL, \
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
    _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM,          AG_SWAP, COLEMAK, QWERTY,  _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ \
    )
};

#else
#error "undefined keymaps"
#endif


#ifdef AUDIO_ENABLE

float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
float tone_plover[][2]     = SONG(PLOVER_SOUND);
float tone_plover_gb[][2]  = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
#endif

// define variables for reactive RGB
bool TOG_STATUS = false;
int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    rgblight_mode(RGB_current_mode);
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case COLEMAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_colemak);
        #endif
        persistent_default_layer_set(1UL<<_COLEMAK);
      }
      return false;
      break;
    case DVORAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_dvorak);
        #endif
        persistent_default_layer_set(1UL<<_DVORAK);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
          //not sure how to have keyboard check mode and set it to a variable, so my work around
          //uses another variable that would be set to true after the first time a reactive key is pressed.
        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
        } else {
          TOG_STATUS = !TOG_STATUS;
          rgblight_mode(16);
        }
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        rgblight_mode(RGB_current_mode);   // revert RGB to initial mode prior to RGB mode change
        TOG_STATUS = false;
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        //not sure how to have keyboard check mode and set it to a variable, so my work around
        //uses another variable that would be set to true after the first time a reactive key is pressed.
        if (TOG_STATUS) { //TOG_STATUS checks is another reactive key currently pressed, only changes RGB mode if returns false
        } else {
          TOG_STATUS = !TOG_STATUS;
          rgblight_mode(15);
        }
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        rgblight_mode(RGB_current_mode);  // revert RGB to initial mode prior to RGB mode change
        layer_off(_RAISE);
        TOG_STATUS = false;
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
        break;
    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
      } else {
        unregister_code(KC_RSFT);
      }
      return false;
      break;
      //led operations - RGB mode change now updates the RGB_current_mode to allow the right RGB mode to be set after reactive keys are released
    case RGB_MOD:
      if (record->event.pressed) {
        rgblight_mode(RGB_current_mode);
        rgblight_step();
        RGB_current_mode = rgblight_config.mode;
      }
      return false;
      break;
    case RGBRST:
      if (record->event.pressed) {
        eeconfig_update_rgblight_default();
        //rgblight_mode(9);
        rgblight_enable();
        //rgblight_sethsv(0, 255, 255);
        RGB_current_mode = rgblight_config.mode;
      }
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    RGB_current_mode = rgblight_config.mode;
}

//SSD1306 OLED init and update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED
void matrix_master_OLED_init (void) {
    TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
    iota_gfx_init();   // turns on the display
}

void matrix_scan_user(void) {
  logo_render();
  // iota_gfx_task();  // this is what updates the display continuously
}
#endif

#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
}

void shutdown_user()
{
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif

/*
 * Macro definition
 */
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    if (!eeconfig_is_enabled()) {
      eeconfig_init();
    }

    switch (id) {
      case KC_SAMPLEMACRO:
        if (record->event.pressed){
          return MACRO (I(10), T(H), T(E), T(L), T(L), T(O), T(SPACE), T(W), T(O), T(R), T(L), T(D), END);
        }

    }

    return MACRO_NONE;
}
