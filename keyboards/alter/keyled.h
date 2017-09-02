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

#ifndef KEYLED_H
#define KEYLED_H

#include <stdint.h>

// led driver address
#define LEFT_LED_ADDR        0x3F
#define RIGHT_LED_ADDR        0x3D
// led iref
#define KEYLED_DEF_IREF 50
#define KEYLED_CTRL_IREF 2
#define KEYLED_MAX_IREF 100

void keyled_fill(uint8_t r, uint8_t g, uint8_t b);
void keyled_fill_hsv(unsigned int h, float s, float v);
void keyled_dot(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void keyled_dot_hsv(uint8_t x, uint8_t y, unsigned int h, float s, float v);
void keyled_fillcheck(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
void keyled_fillcheck_hsv(unsigned int h1, float s1, float v1, unsigned int h2, float s2, float v2);
void keyled_set_iref(uint8_t _iref);
void keyled_up_iref(uint8_t val);
void keyled_down_iref(uint8_t val);
void keyled_off_iref(void);
void keyled_on_iref(void);
void write_keyled(uint8_t row);
void alloff_keyled(uint8_t row);
void hsv2rgb(unsigned int  h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b);


#endif
