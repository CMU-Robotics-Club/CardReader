/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "kb_usb_hid.h"

uint8 bSNstring[16u] = {0x0Eu, 0x03u, 'F', 0u, 'W', 0u, 'S', 0u, 'N', 0u, '0', 0u, '1', 0u};

// Look-up table stored in code memory that we use to convert from ASCII
// characters to HID key codes.
uint8_t kb_key_from_ascii[128] =
{
    0,                 // 0x00 Null
    0,                 // 0x01
    0,                 // 0x02
    0,                 // 0x03
    0,                 // 0x04
    0,                 // 0x05
    0,                 // 0x06
    0,                 // 0x07
    KB_KEY_BACKSPACE,     // 0x08 Backspace
    KB_KEY_TAB,           // 0x09 Horizontal Tab
    KB_KEY_RETURN,        // 0x0A Line Feed
    0,                 // 0x0B
    0,                 // 0x0C
    KB_KEY_RETURN,        // 0x0D Carriage return
    0,                 // 0x0E
    0,                 // 0x0F
    0,                 // 0x10
    0,                 // 0x11
    0,                 // 0x12
    0,                 // 0x13
    0,                 // 0x14
    0,                 // 0x15
    0,                 // 0x16
    0,                 // 0x17
    0,                 // 0x18
    0,                 // 0x19
    0,                 // 0x1A
    KB_KEY_ESCAPE,        // 0x1B Escape
    0,                 // 0x1C
    0,                 // 0x1D
    0,                 // 0x1E
    0,                 // 0x1F
    KB_KEY_SPACE,         // 0x20
    KB_KEY_1,             // 0x21 !
    KB_KEY_APOSTROPHE,    // 0x22 "
    KB_KEY_3,             // 0x23 #
    KB_KEY_4,             // 0x24 $
    KB_KEY_5,             // 0x25 %
    KB_KEY_7,             // 0x26 &
    KB_KEY_APOSTROPHE,    // 0x27 '
    KB_KEY_9,             // 0x28 (
    KB_KEY_0,             // 0x29 )
    KB_KEY_8,             // 0x2A *
    KB_KEY_EQUAL,         // 0x2B +
    KB_KEY_COMMA,         // 0x2C ,
    KB_KEY_MINUS,         // 0x2D -
    KB_KEY_PERIOD,        // 0x2E .
    KB_KEY_SLASH,         // 0x2F /
    KB_KEY_0,             // 0x30 0
    KB_KEY_1,             // 0x31 1
    KB_KEY_2,             // 0x32 2
    KB_KEY_3,             // 0x33 3
    KB_KEY_4,             // 0x34 4
    KB_KEY_5,             // 0x35 5
    KB_KEY_6,             // 0x36 6
    KB_KEY_7,             // 0x37 7
    KB_KEY_8,             // 0x38 8
    KB_KEY_9,             // 0x39 9
    KB_KEY_SEMICOLON,     // 0x3A :
    KB_KEY_SEMICOLON,     // 0x3B ;
    KB_KEY_COMMA,         // 0x3C <
    KB_KEY_EQUAL,         // 0x3D =
    KB_KEY_PERIOD,        // 0x3E >
    KB_KEY_SLASH,         // 0x3F ?
    KB_KEY_3,             // 0x40 @
    KB_KEY_A,             // 0x41 A
    KB_KEY_B,             // 0x42 B
    KB_KEY_C,             // 0x43 C
    KB_KEY_D,             // 0x44 D
    KB_KEY_E,             // 0x45 E
    KB_KEY_F,             // 0x46 F
    KB_KEY_G,             // 0x47 G
    KB_KEY_H,             // 0x48 H
    KB_KEY_I,             // 0x49 I
    KB_KEY_J,             // 0x4A J
    KB_KEY_K,             // 0x4B K
    KB_KEY_L,             // 0x4C L
    KB_KEY_M,             // 0x4D M
    KB_KEY_N,             // 0x4E N
    KB_KEY_O,             // 0x4F O
    KB_KEY_P,             // 0x50 P
    KB_KEY_Q,             // 0x51 Q
    KB_KEY_R,             // 0x52 R
    KB_KEY_S,             // 0x53 S
    KB_KEY_T,             // 0x55 T
    KB_KEY_U,             // 0x55 U
    KB_KEY_V,             // 0x56 V
    KB_KEY_W,             // 0x57 W
    KB_KEY_X,             // 0x58 X
    KB_KEY_Y,             // 0x59 Y
    KB_KEY_Z,             // 0x5A Z
    KB_KEY_BRACKET_LEFT,  // 0x5B [
    KB_KEY_BACKSLASH,     // 0x5C '\'
    KB_KEY_BRACKET_RIGHT, // 0x5D ]
    KB_KEY_6,             // 0x5E ^
    KB_KEY_MINUS,         // 0x5F _
    KB_KEY_GRAVE,         // 0x60 `
    KB_KEY_A,             // 0x61 a
    KB_KEY_B,             // 0x62 b
    KB_KEY_C,             // 0x63 c
    KB_KEY_D,             // 0x66 d
    KB_KEY_E,             // 0x65 e
    KB_KEY_F,             // 0x66 f
    KB_KEY_G,             // 0x67 g
    KB_KEY_H,             // 0x68 h
    KB_KEY_I,             // 0x69 i
    KB_KEY_J,             // 0x6A j
    KB_KEY_K,             // 0x6B k
    KB_KEY_L,             // 0x6C l
    KB_KEY_M,             // 0x6D m
    KB_KEY_N,             // 0x6E n
    KB_KEY_O,             // 0x6F o
    KB_KEY_P,             // 0x70 p
    KB_KEY_Q,             // 0x71 q
    KB_KEY_R,             // 0x72 r
    KB_KEY_S,             // 0x73 s
    KB_KEY_T,             // 0x75 t
    KB_KEY_U,             // 0x75 u
    KB_KEY_V,             // 0x76 v
    KB_KEY_W,             // 0x77 w
    KB_KEY_X,             // 0x78 x
    KB_KEY_Y,             // 0x79 y
    KB_KEY_Z,             // 0x7A z
    KB_KEY_BRACKET_LEFT,  // 0x7B {
    KB_KEY_BACKSLASH,     // 0x7C |
    KB_KEY_BRACKET_RIGHT, // 0x7D }
    KB_KEY_GRAVE,         // 0x7E ~
    KB_KEY_DELETE         // 0x7F Delete
};

uint8 kb_mod_from_ascii[128] =
{
    0,                 // 0x00 Null
    0,                 // 0x01
    0,                 // 0x02
    0,                 // 0x03
    0,                 // 0x04
    0,                 // 0x05
    0,                 // 0x06
    0,                 // 0x07
    0,                 // 0x08 Backspace
    0,                 // 0x09 Horizontal Tab
    0,                 // 0x0A Line Feed
    0,                 // 0x0B
    0,                 // 0x0C
    0,                 // 0x0D Carriage return
    0,                 // 0x0E
    0,                 // 0x0F
    0,                 // 0x10
    0,                 // 0x11
    0,                 // 0x12
    0,                 // 0x13
    0,                 // 0x14
    0,                 // 0x15
    0,                 // 0x16
    0,                 // 0x17
    0,                 // 0x18
    0,                 // 0x19
    0,                 // 0x1A
    0,                 // 0x1B Escape
    0,                 // 0x1C
    0,                 // 0x1D
    0,                 // 0x1E
    0,                 // 0x1F
    0,                 // 0x20
    KB_MOD_LSHIFT,     // 0x21 !
    KB_MOD_LSHIFT,     // 0x22 "
    KB_MOD_LSHIFT,     // 0x23 #
    KB_MOD_LSHIFT,     // 0x24 $
    KB_MOD_LSHIFT,     // 0x25 %
    KB_MOD_LSHIFT,     // 0x26 &
    0,                 // 0x27 '
    KB_MOD_LSHIFT,     // 0x28 (
    KB_MOD_LSHIFT,     // 0x29 )
    KB_MOD_LSHIFT,     // 0x2A *
    KB_MOD_LSHIFT,     // 0x2B +
    0,                 // 0x2C ,
    0,                 // 0x2D -
    0,                 // 0x2E .
    0,                 // 0x2F /
    0,                 // 0x30 0
    0,                 // 0x31 1
    0,                 // 0x32 2
    0,                 // 0x33 3
    0,                 // 0x34 4
    0,                 // 0x35 5
    0,                 // 0x36 6
    0,                 // 0x37 7
    0,                 // 0x38 8
    0,                 // 0x39 9
    KB_MOD_LSHIFT,     // 0x3A :
    0,                 // 0x3B ;
    KB_MOD_LSHIFT,     // 0x3C <
    0,                 // 0x3D =
    KB_MOD_LSHIFT,     // 0x3E >
    KB_MOD_LSHIFT,     // 0x3F ?
    KB_MOD_LSHIFT,     // 0x40 @
    KB_MOD_LSHIFT,     // 0x41 A
    KB_MOD_LSHIFT,     // 0x42 B
    KB_MOD_LSHIFT,     // 0x43 C
    KB_MOD_LSHIFT,     // 0x44 D
    KB_MOD_LSHIFT,     // 0x45 E
    KB_MOD_LSHIFT,     // 0x46 F
    KB_MOD_LSHIFT,     // 0x47 G
    KB_MOD_LSHIFT,     // 0x48 H
    KB_MOD_LSHIFT,     // 0x49 I
    KB_MOD_LSHIFT,     // 0x4A J
    KB_MOD_LSHIFT,     // 0x4B K
    KB_MOD_LSHIFT,     // 0x4C L
    KB_MOD_LSHIFT,     // 0x4D M
    KB_MOD_LSHIFT,     // 0x4E N
    KB_MOD_LSHIFT,     // 0x4F O
    KB_MOD_LSHIFT,     // 0x50 P
    KB_MOD_LSHIFT,     // 0x51 Q
    KB_MOD_LSHIFT,     // 0x52 R
    KB_MOD_LSHIFT,     // 0x53 S
    KB_MOD_LSHIFT,     // 0x55 T
    KB_MOD_LSHIFT,     // 0x55 U
    KB_MOD_LSHIFT,     // 0x56 V
    KB_MOD_LSHIFT,     // 0x57 W
    KB_MOD_LSHIFT,     // 0x58 X
    KB_MOD_LSHIFT,     // 0x59 Y
    KB_MOD_LSHIFT,     // 0x5A Z
    0,                 // 0x5B [
    0,                 // 0x5C '\'
    0,                 // 0x5D ]
    KB_MOD_LSHIFT,     // 0x5E ^
    KB_MOD_LSHIFT,     // 0x5F _
    0,                 // 0x60 `
    0,                 // 0x61 a
    0,                 // 0x62 b
    0,                 // 0x63 c
    0,                 // 0x66 d
    0,                 // 0x65 e
    0,                 // 0x66 f
    0,                 // 0x67 g
    0,                 // 0x68 h
    0,                 // 0x69 i
    0,                 // 0x6A j
    0,                 // 0x6B k
    0,                 // 0x6C l
    0,                 // 0x6D m
    0,                 // 0x6E n
    0,                 // 0x6F o
    0,                 // 0x70 p
    0,                 // 0x71 q
    0,                 // 0x72 r
    0,                 // 0x73 s
    0,                 // 0x75 t
    0,                 // 0x75 u
    0,                 // 0x76 v
    0,                 // 0x77 w
    0,                 // 0x78 x
    0,                 // 0x79 y
    0,                 // 0x7A z
    KB_MOD_LSHIFT,     // 0x7B {
    KB_MOD_LSHIFT,     // 0x7C |
    KB_MOD_LSHIFT,     // 0x7D }
    KB_MOD_LSHIFT,     // 0x7E ~
    0                  // 0x7F Delete
};


void kb_putc(char c) {
    kb_data_t kb_data = {0x0, 0x0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    
    // send a keypress for this character if it is in the ascii range
    if((c & ~KB_ASCII_MASK) == 0) {
        // press the key
        kb_data.mods = kb_mod_from_ascii[(uint8_t)c];
        kb_data.keys[0] = kb_key_from_ascii[(uint8_t)c];
        USBFS_LoadInEP(KB_IN_ENDPOINT, (uint8_t *)(&kb_data), KB_IN_DATA_LEN);
        while (0u == USBFS_GetEPAckState(KB_IN_ENDPOINT)) {}
        
        // depress the key
        kb_data.mods = 0x00;
        kb_data.keys[0] = 0x00;
        USBFS_LoadInEP(KB_IN_ENDPOINT, (uint8_t *)(&kb_data), KB_IN_DATA_LEN);
        while (0u == USBFS_GetEPAckState(KB_IN_ENDPOINT)) {}
    }
}


void kb_puts(char *s) {
    int s_len = strlen(s);
    for(int i=0; i < s_len; i++) {
        kb_putc(s[i]);
    }
}


void kb_init() {
    /* Set user-defined Serial Number string descriptor. */
    USBFS_SerialNumString(bSNstring);

    /* Start USBFS operation with 5-V operation. */
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);

    /* Wait for device to enumerate */
    while (0u == USBFS_GetConfiguration()) {}

    // start the USB transfers by sending all keys depressed
    kb_data_t kb_data = {0x0, 0x0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
    USBFS_LoadInEP(KB_IN_ENDPOINT, (uint8_t *)(&kb_data), KB_IN_DATA_LEN);
    while (0u == USBFS_GetEPAckState(KB_IN_ENDPOINT)) {}
}


/* [] END OF FILE */
