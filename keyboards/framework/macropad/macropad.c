// Copyright 2022 Framework Computer
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "raw_hid.h"

#if defined(RGB_MATRIX_ENABLE)
const is31_led g_is31_leds[RGB_MATRIX_LED_COUNT] = {
/* Refer to IS31 manual for these locations
 *   driver
 *   |   R location
 *   |   |         G location
 *   |   |         |         B location
 *   |   |         |         | */
    // Re-arranged in LED ID order so it matches with the physical location array
    {0,  CS6_SW3,  CS5_SW3,  CS4_SW3}, // LED 6 = 1,1
    {0,  CS3_SW3,  CS2_SW3,  CS1_SW3}, // LED 3 = 2,1
    {0, CS18_SW3, CS17_SW3, CS16_SW3}, // LED 23 = 3,1
    {0, CS15_SW3, CS14_SW3, CS13_SW3}, // LED 18 = 4,1

    {0,  CS6_SW1,  CS5_SW1,  CS4_SW1}, // LED 5 = 1,2
    {0,  CS3_SW1,  CS2_SW1,  CS1_SW1}, // LED 1 = 2,2
    {0, CS18_SW1, CS17_SW1, CS16_SW1}, // LED 21 = 3,2
    {0, CS15_SW1, CS14_SW1, CS13_SW1}, // LED 19 = 4,2

    {0,  CS6_SW2,  CS5_SW2,  CS4_SW2}, // LED 8 = 1,3
    {0,  CS3_SW2,  CS2_SW2,  CS1_SW2}, // LED 2 = 2,3
    {0, CS18_SW2, CS17_SW2, CS16_SW2}, // LED 22 = 3,3
    {0, CS15_SW2, CS14_SW2, CS13_SW2}, // LED 17 = 4,3


    {0,  CS6_SW4,  CS5_SW4,  CS4_SW4}, // LED 7 = 1,4
    {0,  CS3_SW4,  CS2_SW4,  CS1_SW4}, // LED 4 = 2,4
    {0, CS18_SW4, CS17_SW4, CS16_SW4}, // LED 24 = 3,4
    {0, CS15_SW4, CS14_SW4, CS13_SW4}, // LED 20 = 4,4

    {0,  CS9_SW2,  CS8_SW2,  CS7_SW2}, // LED 10 = 1,5
    {0,  CS9_SW4,  CS8_SW4,  CS7_SW4}, // LED 12 = 2,5
    {0, CS12_SW4, CS11_SW4, CS10_SW4}, // LED 16 = 3,5
    {0, CS12_SW2, CS11_SW2, CS10_SW2}, // LED 14 = 4,5

    {0,  CS9_SW1,  CS8_SW1,  CS7_SW1}, // LED 9 = 1,6
    {0,  CS9_SW3,  CS8_SW3,  CS7_SW3}, // LED 11 = 2,6
    {0, CS12_SW3, CS11_SW3, CS10_SW3}, // LED 15 = 3,6
    {0, CS12_SW1, CS11_SW1, CS10_SW1}, // LED 13 = 4,6


};

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  {     4,      7,      6,      9,      0,      1,      3,     11, },
  {     8,     20,     21,     23,     15,     14,     18,     19, },
  {    12,      5,      2, NO_LED,     17,     16,     13,     10, },
  {NO_LED, NO_LED, NO_LED, NO_LED,     22, NO_LED, NO_LED, NO_LED, },
}, {
  // LED Index to Physical Position
  {  73,  10 }, // LED 1
  {  73,  24 }, // LED 2
  {  73,   0 }, // LED 3
  {  73,  37 }, // LED 4
  {   0,  10 }, // LED 5
  {   0,   0 }, // LED 6
  {   0,  37 }, // LED 7
  {   0,  24 }, // LED 8
  {   0,  64 }, // LED 9
  {   0,  50 }, // LED 10
  {  73,  64 }, // LED 11
  {  73,  50 }, // LED 12
  { 223,  64 }, // LED 13
  { 223,  50 }, // LED 14
  { 150,  64 }, // LED 15
  { 150,  50 }, // LED 16
  { 223,  24 }, // LED 17
  { 223,   0 }, // LED 18
  { 223,  10 }, // LED 19
  { 223,  37 }, // LED 20
  { 150,  10 }, // LED 21
  { 150,  24 }, // LED 22
  { 150,   0 }, // LED 23
  { 150,  37 }, // LED 24
}, {
  // LED Index to Flag
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
} };
#endif

void keyboard_post_init_user(void) {
  // setup as sane layout
  layer_on(_FACTORY);

  rgb_matrix_enable();
  rgb_matrix_mode(0);
  rgb_matrix_set_color_all(0, 0, 0);
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    
    int length = RAW_EPSIZE;
    uint8_t response[length];
    memset(response, 0, length);
    response[0] = 'P';
    // response[1] = record->event.key.row;
    // response[2] = record->event.key.col;
    response[1] = keycode >> 8;
    response[2] = keycode & 0xFF;
    response[3] = record->event.pressed ? 1 : 0;

    // TODO - can this report the pressed state of all keys?

    raw_hid_send(response, length);

    return false;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {

    if(data[0] == 0x0c) { // check if firmware is correct
        uint8_t response[length];
        memset(response, 0, length);
        response[0] = 0x0c;
        response[1] = 1;

        raw_hid_send(response, length);
    } else if (data[0] == 0xff && data[1] == 0xee && data[2] == 0xdd ) {
        bootloader_jump();
    } else if (data[0] == 0x0b) {
        rgb_matrix_set_color_all(data[1], data[2], data[3]);
    } else if (data[0] == 0x0f) {
        uint8_t key_x = data[1];
        uint8_t key_y = data[2];
        if (key_x <= 0 || key_x > 4) {
            return;
        }
         if (key_y <= 0 || key_y > 6) {
            return;
        }

        uint8_t index = (key_x-1) + (key_y - 1) * 4;

        rgb_matrix_set_color(index, data[3], data[4], data[5]);
    }
}