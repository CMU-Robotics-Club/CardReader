/** @file kb_usb_hid.h
 *  @brief header for a USB HID keyboard for PSoc
 *  @author Ian Hartwig
 *
 *  Supports standard C IO functions for ascii strings like putc and puts.
 *  USB output calls are blocking until the data is acknoledged.
 */

#include <project.h>

#define USBFS_DEVICE         (0u)

#define KB_IN_ENDPOINT       (1u)
#define KB_IN_DATA_LEN       (8u)
#define KB_MAX_ROLLOVER      (6)
#define KB_ASCII_MASK        (0x7Fu)

#define KB_KEY_A             (0x04u)
#define KB_KEY_B             (0x05u)
#define KB_KEY_C             (0x06u)
#define KB_KEY_D             (0x07u)
#define KB_KEY_E             (0x08u)
#define KB_KEY_F             (0x09u)
#define KB_KEY_G             (0x0Au)
#define KB_KEY_H             (0x0Bu)
#define KB_KEY_I             (0x0Cu)
#define KB_KEY_J             (0x0Du)
#define KB_KEY_K             (0x0Eu)
#define KB_KEY_L             (0x0Fu)
#define KB_KEY_M             (0x10u)
#define KB_KEY_N             (0x11u)
#define KB_KEY_O             (0x12u)
#define KB_KEY_P             (0x13u)
#define KB_KEY_Q             (0x14u)
#define KB_KEY_R             (0x15u)
#define KB_KEY_S             (0x16u)
#define KB_KEY_T             (0x17u)
#define KB_KEY_U             (0x18u)
#define KB_KEY_V             (0x19u)
#define KB_KEY_W             (0x1Au)
#define KB_KEY_X             (0x1Bu)
#define KB_KEY_Y             (0x1Cu)
#define KB_KEY_Z             (0x1Du)
#define KB_KEY_1             (0x1Eu)
#define KB_KEY_2             (0x1Fu)
#define KB_KEY_3             (0x20u)
#define KB_KEY_4             (0x21u)
#define KB_KEY_5             (0x22u)
#define KB_KEY_6             (0x23u)
#define KB_KEY_7             (0x24u)
#define KB_KEY_8             (0x25u)
#define KB_KEY_9             (0x26u)
#define KB_KEY_0             (0x27u)
#define KB_KEY_RETURN        (0x28u)
#define KB_KEY_ESCAPE        (0x29u)
#define KB_KEY_BACKSPACE     (0x2Au)
#define KB_KEY_TAB           (0x2Bu)
#define KB_KEY_SPACE         (0x2Cu)
#define KB_KEY_MINUS         (0x2Du)
#define KB_KEY_EQUAL         (0x2Eu)
#define KB_KEY_BRACKET_LEFT  (0x2Fu)
#define KB_KEY_BRACKET_RIGHT (0x30u)
#define KB_KEY_BACKSLASH     (0x31u)
#define KB_KEY_SEMICOLON     (0x33u)
#define KB_KEY_APOSTROPHE    (0x34u)
#define KB_KEY_GRAVE         (0x35u)
#define KB_KEY_COMMA         (0x36u)
#define KB_KEY_PERIOD        (0x37u)
#define KB_KEY_SLASH         (0x38u)
#define KB_KEY_DELETE        (0x63u)

#define KB_MOD_LCTRL        (0x01u)
#define KB_MOD_LSHIFT       (0x02u)
#define KB_MOD_LALT         (0x04u)
#define KB_MOD_LGUI         (0x08u)
#define KB_MOD_RCTRL        (0x10u)
#define KB_MOD_RSHIFT       (0x20u)
#define KB_MOD_RALT         (0x40u)
#define KB_MOD_RGUI         (0x80u)

/** @brief report format for 6 key rollover keyboard (boot device) */
typedef struct kb_data {
    uint8_t mods;
    uint8_t reserved;
    uint8_t keys[KB_MAX_ROLLOVER];
} kb_data_t;

/** @brief send a character to the host
 *
 *  Blocks until USB ack is recieved for this character.
 *
 *  @arg c the character to send
 */
void kb_putc(char c);

/** @brief send a C string to the host
 *
 *  Blocks until USB ack is recieved for all characters in string. This string
 *  should only contain ascii characters, not generic data (must allow strlen).
 *
 *  @arg s the string to send
 */
void kb_puts(char *s);

/** @brief initialize the USB HID interface
 *
 *  Blocks until USB ack is recieved for init.
 */
void kb_init(void);

/* [] END OF FILE */
