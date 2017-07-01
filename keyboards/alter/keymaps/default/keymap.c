/* Copyright 2017 MakotoKurauchi <http://yushakobo.jp/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 #include "alter.h"
 #include "keyled.h"

 enum alter_layers {
   _QWERTY,
   _LOWER,
   _RAISE,
   _ADJUST,
   _NumLayers
 };

 enum alter_keycodes {
   LEDIUP = SAFE_RANGE,
   LEDIDN,
   LEDHUP,
   LEDHDN
 };

 // Fillers to make layering more clear
 #define _______ KC_TRNS
 #define XXXXXXX KC_NO


 #ifdef KEYLED_ENABLE

 // base color
 #define KEYLED_BASE_COLOR_HSV 205
 #define KEYLED_CTRL_H 10

 unsigned int BaseColorH = KEYLED_BASE_COLOR_HSV;

 #endif

 const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

 /* Qwerty
  * ,-----------------------------------------.             ,-----------------------------------------.
  * | Tab  |   Q  |   W  |   E  |   R  |   T  |             |   Y  |   U  |   I  |   O  |   P  | Bksp |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * | Ctrl |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  |  "   |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * | Shift|   Z  |   X  |   C  |   V  |   B  |             |   N  |   M  |   ,  |   .  |   /  |Enter |
  * `------+------+------+------+------+------+-------------+------+------+------+------+------+------'
  *        |Adjust| Esc  | Alt  | GUI  |Lower |Space |Space |Raise | Left | Down |  Up  |Right |
  *        `-----------------------------------------------------------------------------------'
  */
 [_QWERTY] = KEYMAP( \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT , \
   MO(_ADJUST),KC_ESC,KC_LALT, KC_LGUI, MO(_LOWER),KC_SPC, KC_SPC, MO(_RAISE),KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
 ),


 /* Lower
  * ,-----------------------------------------.             ,-----------------------------------------.
  * |   ~  |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |   *  |   (  |   )  | Bksp |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   _  |   +  |      |   \  |  |   |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |  F7  |  F8  |  F9  |  F10 |  F11 |             |  F12 |ISO ~ |ISO | |      |      |      |
  * `------+------+------+------+------+------+-------------+------+------+------+------+------+------'
  *        |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
  *        `-----------------------------------------------------------------------------------'
  */
 [_LOWER] = KEYMAP( \
   KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC, \
   _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, \
   _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,S(KC_NUHS),S(KC_NUBS),_______, _______, _______, \
   _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
 ),


 /* Raise
  * ,-----------------------------------------.             ,-----------------------------------------.
  * |   `  |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | Bksp |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |   -  |   =  |   [  |   ]  |  \   |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |  F7  |  F8  |  F9  |  F10 |  F11 |             |  F12 |ISO # |ISO / |      |      |      |
  * `------+------+------+------+------+------+-------------+------+------+------+------+------+------'
  *        |      |      |      |      |      |      |      |      | Next | Vol- | Vol+ | Play |
  *        `-----------------------------------------------------------------------------------'
  */
 [_RAISE] = KEYMAP( \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, \
   _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, \
   _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, _______, _______, _______, \
   _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY \
 ),


 /* Adjust
  * ,-----------------------------------------.             ,-----------------------------------------.
  * |      | Reset|      |      |      |      |             |      |      |      |      |      |  Del |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |      |      |      |      |      |             |      |      |      |      |      |      |
  * |------+------+------+------+------+------|             |------+------+------+------+------+------|
  * |      |      |      |      |      |      |             |      |      |      |      |      |      |
  * `------+------+------+------+------+------+-------------+------+------+------+------+------+------'
  *        |      |      |      |      |      |      |      |      |LEDHDN|LEDIDN|LEDIUP|LEDHUP|
  *        `-----------------------------------------------------------------------------------'
  */
 [_ADJUST] =  KEYMAP( \
   _______, RESET,   _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL, \
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
   _______, _______, _______, _______, _______, _______, _______, _______, LEDHDN, LEDIDN, LEDIUP, LEDHUP \
 )

 };

 void keyled_layer(uint16_t layer, unsigned int h, float s, float v) {
 	for (uint8_t i=0; i < MATRIX_ROWS; i++) {
 		for (uint8_t j=0; j < MATRIX_COLS; j++) {
       if(keymaps[layer][i][j]!=_______){
         keyled_dot_hsv(j, i, h, s, v);
       }
 		}
 	}
 }


 const uint16_t PROGMEM fn_actions[] = {

 };

 const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
 {
   // MACRODOWN only works in this function
       switch(id) {
         case 0:
           if (record->event.pressed) {
             register_code(KC_RSFT);
           } else {
             unregister_code(KC_RSFT);
           }
         break;
       }
     return MACRO_NONE;
 };


 void matrix_init_user(void) {
   #ifdef KEYLED_ENABLE
   	keyled_fill_hsv(BaseColorH, 1, 1);
   	keyled_set_iref(KEYLED_DEF_IREF);
   #endif
 }

 void matrix_scan_user(void) {
   #ifdef KEYLED_ENABLE

   keyled_fill_hsv(BaseColorH, 1, 1);

   for(uint8_t i=0; i<_NumLayers; i++){
     if( IS_LAYER_ON(i) ){
       keyled_layer(i, BaseColorH+(360/_NumLayers)*i, 1, 1);
     }
   }

   keyled_dot(0, 0, 255, 255, 255);

   #endif
 }

 bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   switch (keycode) {
     case LEDIUP:
       if (record->event.pressed) {
         #ifdef KEYLED_ENABLE
           keyled_up_iref(KEYLED_CTRL_IREF);
         #endif
       }
       return false;
       break;
     case LEDIDN:
       if (record->event.pressed) {
         #ifdef KEYLED_ENABLE
           keyled_down_iref(KEYLED_CTRL_IREF);
         #endif
       }
       return false;
       break;
     case LEDHUP:
       if (record->event.pressed) {
         #ifdef KEYLED_ENABLE
           BaseColorH += KEYLED_CTRL_H;
           keyled_fill_hsv(BaseColorH, 1, 1);
         #endif
       }
       return false;
       break;
     case LEDHDN:
       if (record->event.pressed) {
         #ifdef KEYLED_ENABLE
           if((BaseColorH - KEYLED_CTRL_H)<0){
             BaseColorH = 360 - KEYLED_CTRL_H ;
           }else{
             BaseColorH -= KEYLED_CTRL_H;
           }
           keyled_fill_hsv(BaseColorH, 1, 1);
         #endif
       }
       return false;
       break;
   }

   return true;
 }

 void led_set_user(uint8_t usb_led) {

 }
