/*
Copyright 2017 MakotoKurauchi <http://yushakobo.jp/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "keyled.h"
#include "alter.h"
#include "pca9956.h"


uint8_t Buffer[MATRIX_ROWS][MATRIX_COLS][3]={} ;
uint8_t Iref = 0 ;


// Fill all with the same color
void keyled_fill(uint8_t r, uint8_t g, uint8_t b) {
	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		for (uint8_t j=0; j < MATRIX_COLS; j++) {
			Buffer[i][j][0] = r;
			Buffer[i][j][1] = g;
			Buffer[i][j][2] = b;
		}
	}
}
// hsv ver.
void keyled_fill_hsv(unsigned int h, float s, float v) {
	uint8_t r, g, b;

	hsv2rgb(h, s, v, &r, &g, &b);

	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		for (uint8_t j=0; j < MATRIX_COLS; j++) {
			Buffer[i][j][0] = r;
			Buffer[i][j][1] = g;
			Buffer[i][j][2] = b;
		}
	}
}

// Write a point at the specified position
void keyled_dot(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b){
  Buffer[y][x][0] = r;
  Buffer[y][x][1] = g;
  Buffer[y][x][2] = b;
}
// hsv ver.
void keyled_dot_hsv(uint8_t x, uint8_t y, unsigned int h, float s, float v){
  uint8_t r, g, b;

	hsv2rgb(h, s, v, &r, &g, &b);

  Buffer[y][x][0] = r;
  Buffer[y][x][1] = g;
  Buffer[y][x][2] = b;
}


// Fill the check pattern
void keyled_fillcheck(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2){

	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		for (uint8_t j=0; j < MATRIX_COLS; j++) {
			if((i+j)%2==0){
				Buffer[i][j][0] = r1;
				Buffer[i][j][1] = g1;
				Buffer[i][j][2] = b1;
			}else{
				Buffer[i][j][0] = r2;
				Buffer[i][j][1] = g2;
				Buffer[i][j][2] = b2;
			}
		}
	}
}
// hsv ver.
void keyled_fillcheck_hsv(unsigned int h1, float s1, float v1, unsigned int h2, float s2, float v2){
	uint8_t r1, g1, b1;
	uint8_t r2, g2, b2;

	hsv2rgb(h1, s1, v1, &r1, &g1, &b1);
	hsv2rgb(h2, s2, v2, &r2, &g2, &b2);

	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
		for (uint8_t j=0; j < MATRIX_COLS; j++) {
			if((i+j)%2==0){
				Buffer[i][j][0] = r1;
				Buffer[i][j][1] = g1;
				Buffer[i][j][2] = b1;
			}else{
				Buffer[i][j][0] = r2;
				Buffer[i][j][1] = g2;
				Buffer[i][j][2] = b2;
			}
		}
	}
}


// Set the electric current value (0-255)
void keyled_set_iref(uint8_t _iref) {
	Iref = _iref;
	PCA9956_iref_all(LEFT_LED_ADDR, Iref);
	PCA9956_iref_all(RIGHT_LED_ADDR, Iref);
}

void keyled_up_iref(uint8_t val) {
	if((Iref+val)>KEYLED_MAX_IREF){
		Iref = KEYLED_MAX_IREF;
	}else{
		Iref += val;
	}
	keyled_set_iref(Iref);
}

void keyled_down_iref(uint8_t val) {
	if((Iref-val)<0){
		Iref = 0;
	}else{
		Iref -= val;
	}
	keyled_set_iref(Iref);
}

// Write the color of the specified line
void write_keyled(uint8_t row) {
	uint8_t led_l[(MATRIX_COLS - MATRIX_COLS_SPLIT) * 3] = {};
  uint8_t led_r[MATRIX_COLS_SPLIT * 3] = {};

	// Convert to single array
	for (uint8_t i = 0; i < MATRIX_COLS; i++) {
    if(i<MATRIX_COLS_SPLIT){
      led_l[i * 3    ] = Buffer[row][i][0];
      led_l[i * 3 + 1] = Buffer[row][i][1];
      led_l[i * 3 + 2] = Buffer[row][i][2];
    }else{
      uint8_t ii = (i-MATRIX_COLS_SPLIT);
      led_r[ii * 3    ] = Buffer[row][i][0];
  		led_r[ii * 3 + 1] = Buffer[row][i][1];
  		led_r[ii * 3 + 2] = Buffer[row][i][2];
    }
	}

	// Write to led driver
	PCA9956_pwm(LEFT_LED_ADDR, 0, led_l, (MATRIX_COLS - MATRIX_COLS_SPLIT) * 3);
	PCA9956_pwm(RIGHT_LED_ADDR, 0, led_r, MATRIX_COLS_SPLIT * 3);
}


// HSV to RGB convert
void hsv2rgb(unsigned int  h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b) {

  h %= 360;
  int k = (int)(h / 60.0);
  float f = h / 60.0f - k;
  uint8_t p1 = (uint8_t)(v * (1 - s) * 255.0);
  uint8_t p2 = (uint8_t)(v * (1 - s * f) * 255.0);
  uint8_t p3 = (uint8_t)(v * (1 - s * (1 - f)) * 255.0);
  uint8_t vi = (uint8_t)(v * 255);
  if (k == 0) {
    *r = vi ;
    *g = p3 ;
    *b = p1;
  }
  if (k == 1) {
    *r = p2 ;
    *g = vi ;
    *b = p1;
  }
  if (k == 2) {
    *r = p1 ;
    *g = vi;
    *b = p3;
  }
  if (k == 3) {
    *r = p1 ;
    *g = p2 ;
    *b = vi;
  }
  if (k == 4) {
    *r = p3 ;
    *g = p1 ;
    *b = vi;
  }
  if (k == 5) {
    *r = vi ;
    *g = p1 ;
    *b = p2;
  }
}
