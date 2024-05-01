// Copyright 2022 Framework Computer
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "factory.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /* Alphabet
     *         ┌────┬────┬────┬────┐
     *  4 keys │ A  │ B  │ C  │ D  │
     *         ├────┼────┼────┼────┤
     *  4 keys │ E  │ F  │ G  │ H  │
     *         ├────┼────┼────┼────┤
     *  4 keys │ I  │ J  │ K  │ L  │
     *         ├────┼────┼────┼────┤
     *  4 keys │ M  │ N  │ O  │ P  │
     *         ├────┼────┼────┼────┤
     *  4 keys │ Q  │ R  │ S  │ T  │
     *         ├────┼────┼────┼────┤
     *  4 keys │ U  │ V  │ W  │ X  │
     *         └────┴────┴────┴────┘
     * 24 total
     */
    [_FACTORY] = LAYOUT(
        0x0101,    0x0201,    0x0301,    0x0401,
        0x0102,    0x0202,    0x0302,    0x0402,
        0x0103,    0x0203,    0x0303,    0x0403,
        0x0104,    0x0204,    0x0304,    0x0404,
        0x0105,    0x0205,    0x0305,    0x0405,
        0x0106,    0x0206,    0x0306,    0x0406
    ),
};

bool led_update_user(led_t led_state) {
    // // Change layer if numlock state changes, either triggered by OS or
    // // by numlock key on this keyboard
    // if (led_state.num_lock) {
    //     layer_off(_FN);
    // } else {
    //     layer_on(_FN);
    // }
    return true;
}

void enable_factory_mode(bool enable) {
    // if (enable)
    //     layer_on(_FACTORY);
    // else
    //     layer_off(_FACTORY);
}
