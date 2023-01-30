// Copyright 2022 m.ki (@telzo2000)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE,
    ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    
  [_QWERTY] = LAYOUT(
    KC_Q,           KC_W,    KC_E,            KC_R,          KC_T,           KC_Y,    KC_U,    KC_I,             KC_O,    KC_P,
    KC_A,           KC_S,    KC_D,            KC_F,          KC_G,           KC_H,    KC_J,    KC_K,             KC_L,    SFT_T(KC_SCLN),
    KC_Z,           KC_X,    KC_C,            KC_V,          KC_B,           KC_N,    KC_M,    KC_COMM,          KC_DOT,  CTL_T(KC_SLSH),
    KC_1,           KC_2,    LALT_T(KC_LGUI), GUI_T(KC_TAB), SFT_T(KC_SPC),  LOWER,   RAISE,   RALT_T(KC_RGUI),  KC_MPLY, KC_MNXT,
    KC_LEFT,        KC_RIGHT,KC_UP,           KC_DOWN,       KC_VOLD,        KC_VOLU, KC_BSPC, KC_SPC
  ),
  
  [_LOWER] = LAYOUT(
    KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR,  KC_LPRN,KC_RPRN, KC_DEL,
    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,     KC_8,   KC_9,    KC_0,
    _______, _______, _______, KC_EQL,  KC_MINS,  KC_GRV,  KC_QUOT, KC_LBRC,KC_RBRC, KC_BSLS,
    _______, _______, KC_ESC, KC_ESC,   _______, _______, _______,  KC_MNXT,KC_VOLD, KC_VOLU,
    KC_LEFT, KC_RIGHT,KC_UP,  KC_DOWN,  KC_BRID,   KC_BRIU,    KC_C,     KC_D
  ) ,
 
  
  [_RAISE] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
    KC_F11,  KC_F12,  KC_LALT, KC_LGUI, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
    _______, _______, _______, _______, _______, _______, KC_NUHS, KC_BTN1, KC_BTN2, KC_MS_WH_UP,
    _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU,
    KC_LEFT,KC_RIGHT,KC_UP,KC_DOWN,KC_A,KC_B,             KC_BSPC, KC_SPC
  )        
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 1},
    .pressed = false
};

keyevent_t encoder2_ccw = {
    .key = (keypos_t){.row = 4, .col = 2},
    .pressed = false
};

keyevent_t encoder2_cw = {
    .key = (keypos_t){.row = 4, .col = 3},
    .pressed = false
};

keyevent_t encoder3_ccw = {
    .key = (keypos_t){.row = 9, .col = 1},
    .pressed = false
};

keyevent_t encoder3_cw = {
    .key = (keypos_t){.row = 9, .col = 0},
    .pressed = false
};

keyevent_t encoder4_ccw = {
    .key = (keypos_t){.row = 9, .col = 3},
    .pressed = false
};

keyevent_t encoder4_cw = {
    .key = (keypos_t){.row = 9, .col = 2},
    .pressed = false
};

static bool lower_pressed = false;
static uint16_t lower_pressed_time = 0;
static bool raise_pressed = false;
static uint16_t raise_pressed_time = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        lower_pressed = true;
        lower_pressed_time = record->event.time;

        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);

        if (lower_pressed && (TIMER_DIFF_16(record->event.time, lower_pressed_time) < TAPPING_TERM)) {
          register_code(KC_BSPC); // for macOS
          unregister_code(KC_BSPC);
        }
        lower_pressed = false;
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        raise_pressed = true;
        raise_pressed_time = record->event.time;

        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);

        if (raise_pressed && (TIMER_DIFF_16(record->event.time, raise_pressed_time) < TAPPING_TERM)) {
          register_code(KC_ENT);
          unregister_code(KC_ENT);
        }
        raise_pressed = false;
      }
      return false;
      break;
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
    default:
      if (record->event.pressed) {
        // reset the flags
        lower_pressed = false;
        raise_pressed = false;
      }
      break;
  }
  return true;
}


void matrix_scan_user(void) {
    if (IS_PRESSED(encoder1_ccw)) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (IS_PRESSED(encoder1_cw)) {
        encoder1_cw.pressed = false;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    }

    if (IS_PRESSED(encoder2_ccw)) {
        encoder2_ccw.pressed = false;
        encoder2_ccw.time = (timer_read() | 1);
        action_exec(encoder2_ccw);
    }

    if (IS_PRESSED(encoder2_cw)) {
        encoder2_cw.pressed = false;
        encoder2_cw.time = (timer_read() | 1);
        action_exec(encoder2_cw);
    }

    if (IS_PRESSED(encoder3_ccw)) {
        encoder3_ccw.pressed = false;
        encoder3_ccw.time = (timer_read() | 1);
        action_exec(encoder3_ccw);
    }

    if (IS_PRESSED(encoder3_cw)) {
        encoder3_cw.pressed = false;
        encoder3_cw.time = (timer_read() | 1);
        action_exec(encoder3_cw);
    }

    if (IS_PRESSED(encoder4_ccw)) {
        encoder4_ccw.pressed = false;
        encoder4_ccw.time = (timer_read() | 1);
        action_exec(encoder4_ccw);
    }

    if (IS_PRESSED(encoder4_cw)) {
        encoder4_cw.pressed = false;
        encoder4_cw.time = (timer_read() | 1);
        action_exec(encoder4_cw);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    } else if (index == 1) {
        if (clockwise) {
            encoder2_cw.pressed = true;
            encoder2_cw.time = (timer_read() | 1);
            action_exec(encoder2_cw);
        } else {
            encoder2_ccw.pressed = true;
            encoder2_ccw.time = (timer_read() | 1);
            action_exec(encoder2_ccw);
        }
    } else if (index == 2) {
        if (clockwise) {
            encoder3_cw.pressed = true;
            encoder3_cw.time = (timer_read() | 1);
            action_exec(encoder3_cw);
        } else {
            encoder3_ccw.pressed = true;
            encoder3_ccw.time = (timer_read() | 1);
            action_exec(encoder3_ccw);
        }
    } else if (index == 3) {
        if (clockwise) {
            encoder4_cw.pressed = true;
            encoder4_cw.time = (timer_read() | 1);
            action_exec(encoder4_cw);
        } else {
            encoder4_ccw.pressed = true;
            encoder4_ccw.time = (timer_read() | 1);
            action_exec(encoder4_ccw);
        }
    }

    return true;
}
