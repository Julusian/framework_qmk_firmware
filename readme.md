# Framework macropad as raw hid device

This is a custom firmware for the framework macropad based on [QMK](https://qmk.fm/).
It builds on the QMK firmware [provided by framework](https://github.com/FrameworkComputer/qmk_firmware), but replaces the normal keyboard functionality with a simple raw hid device. This is intended to be used with an application to take exclusive control and use it similarly to if it were a midi device.

## Flashing

-   Follow the [setup guide](https://docs.qmk.fm/#/newbs_getting_started) for QMK
-   Put the macropad into bootloader mode, by holding 2 + 6 (if it were a numpad) while sliding the trackpad into place (this action is what connects the macropad to usb)
-   Run `qmk flash -kb framework/macropad -km default` to build and flash the firmware

## Restoring original firmware

The process is identical to flashing the firmware, but using a clone of the latest release of the official firmware [from here](https://github.com/FrameworkComputer/qmk_firmware/releases)

## HID protocol

### Received messages

When a key is pressed the macropad will send a hid message of 32 bytes, with the following meanings:

-   Byte 0 = 0x50 - This identifies the type of the message as a press
-   Byte 1 = the x coordinate (1 - 4)
-   Byte 2 = the y coordinate (1 - 6)
-   Byte 3 = whether the button is pressed (1) or released (0)

### Sent messages

All messages sent to the macropad must be 32 bytes in length.

#### Check version:

-   Byte 0 = 0x0c
-   Byte 1 = 0x01 - the revision of the protocol you are expecting to use

This will respond with:

-   Byte 0 = 0x0c
-   Byte 1 = 0x01 - in later revisions this may be greater than 1, to indicate support for newer functionality

#### Set all button leds

-   Byte 0 = 0x0b
-   Byte 1 = red value
-   Byte 2 = green value
-   Byte 3 = blue value

#### Set single button led

-   Byte 0 = 0x0f
-   Byte 1 = the x coordinate (1 - 4)
-   Byte 2 = the y coordinate (1 - 6)
-   Byte 3 = red value
-   Byte 4 = green value
-   Byte 5 = blue value

#### Enter bootloader

This will immediately enter the bootloader, allowing for easier firmware flashing

-   Byte 0 = 0xff
-   Byte 1 = 0xee
-   Byte 2 = 0xdd
