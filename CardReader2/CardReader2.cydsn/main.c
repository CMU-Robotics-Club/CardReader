/*******************************************************************************
 *  @file main.c
 *
 *  @brief write serial numbers from a SM130 RFID module to a USB HID keyboard
 *
 *  Based on Cypress AN58726 - USB HID Intermediate with PSoC 3 and PSoC 5LP
 *
 ******************************************************************************/

#include <project.h>
#include <stdio.h>
#include <string.h>
#include "kb_usb_hid.h"

// #define DEBUG

/** @brief global buffer for keyboard led state */
uint8_t g_led_state = 0x0;

/** @brief max length of an RFID response packet we care about */
#define RFID_PACKET_LEN 15
#define RFID_HEADER 0xFF
#define RFID_RESERVED 0x00
#define RFID_COMMAND_SEEK 0x82
#define RFID_TAG_TYPE_ULTRALIGHT 0x01
#define RFID_TAG_TYPE_1K 0x02
#define RFID_TAG_TYPE_4K 0x03

/** @brief get the upper error byte from 16-bit UART response */
#define UART_GET_ERROR(b) ((uint16_t)b >> 8)
/** @brief get the lower data byte from 16-bit UART response */
#define UART_GET_DATA(b) (b & 0x00FF)

/** @brief uart packet to ask rfid reader to seek for a card */
const uint8_t rfid_command_seek[] = {0xFF, 0x00, 0x01, 0x82, 0x83};

/** @brief sections of the rfid reader UART packet */
typedef enum rfid_meta {
    RFID_META_NONE = 0,
    RFID_META_HEADER,
    RFID_META_RESERVED,
    RFID_META_LENGTH,
    RFID_META_DATA,
    RFID_META_CRC,
} rfid_meta_t;


/** @brief read an rfid packet from the UART interface, blocking until success
 *
 *  Checks for byte errors from the UART hardware and checksum errors over packet.
 */
void rfid_read_packet(uint8_t *buffer, int length_max, int *length_seen_p) {
    #ifdef DEBUG
    char display_buffer[1024];
    #endif
    
    // iteratively process returned packets
    rfid_meta_t rfid_meta_seen = RFID_META_NONE;
    unsigned int rfid_length_expected;
    unsigned int rfid_length_seen;
    while(1) {
        // wait for a new character
        while(UART_GetRxBufferSize() == 0) {}
        // check this byte for hw problems
        uint16_t new_byte = UART_GetByte();
        uint8_t new_byte_error = UART_GET_ERROR(new_byte);
        uint8_t new_byte_data = UART_GET_DATA(new_byte);
        if(new_byte_error != 0) {
            #ifdef DEBUG
            snprintf(display_buffer, 1024, "Got byte 0x%02x with error code 0x%02x!\n",
                     new_byte_data, new_byte_error);
            kb_puts(display_buffer);
            #endif
            // reset UART packet state machine
            rfid_meta_seen = RFID_META_NONE;
        }
        else {
            // UART packet from RFID state machine
            switch(rfid_meta_seen) {
                case RFID_META_NONE:
                    if(new_byte_data == RFID_HEADER) {
                        rfid_meta_seen = RFID_META_HEADER;
                    }
                    // no need to reset. already at first state
                break;
                // expect to see reserved next
                case RFID_META_HEADER:
                    if(new_byte_data == RFID_RESERVED) {
                        rfid_meta_seen = RFID_META_RESERVED;
                    }
                    // reset if we don't see a reserved byte
                    else {
                        rfid_meta_seen = RFID_META_NONE;
                    }
                break;
                // expect to see length next
                case RFID_META_RESERVED:
                    if(new_byte_data <= length_max) {
                        rfid_length_expected = new_byte_data;
                        rfid_length_seen = 0;
                        rfid_meta_seen = RFID_META_LENGTH;
                    }
                    // reset if we see a length longer than we care about
                    else {
                        rfid_meta_seen = RFID_META_NONE;
                    }
                break;
                // expect to read in data next
                case RFID_META_LENGTH:
                    buffer[rfid_length_seen] = new_byte_data;
                    rfid_length_seen++;
                    // check for end of data. doesn't work on 0 data length
                    if(rfid_length_seen >= rfid_length_expected) {
                        rfid_meta_seen = RFID_META_DATA;
                        *length_seen_p = rfid_length_seen;
                    }
                break;
                // expect to see the CRC byte next
                case RFID_META_DATA:
                    // calculate CRC of length and data bytes
                    ;
                    uint8_t csum = 0;
                    csum += rfid_length_seen;
                    for(unsigned int i = 0; i < rfid_length_seen; i++) {
                        csum += buffer[i];
                    }
                    // CRC should match sent byte
                    if(csum == new_byte_data) {
                        #ifdef DEBUG
                        snprintf(display_buffer, 1024, "Got message type 0x%02x with length 0x%02x.\n",
                                 buffer[0], rfid_length_expected);
                        kb_puts(display_buffer);
                        #endif
                        // send the packet back for processing
                        return;
                    }
                    else {
                        #ifdef DEBUG
                        kb_puts("New message failed checksum!");
                        #endif
                    }
                break;
                // hopefully we don't get here! reset parser.
                case RFID_META_CRC:
                default:
                    rfid_meta_seen = RFID_META_NONE;
                break;
            }
        }
    }
}


int main()
{
    CyGlobalIntEnable;
    LED1_Write(0xFFu);
    
    kb_init();
    UART_Start();
    UART_ClearRxBuffer();
    UART_ClearTxBuffer();
            
    char display_buffer[1024];
    uint8_t rfid_packet_buffer[RFID_PACKET_LEN];
    int rfid_packet_length;
    
    while(1) {
        // set the RFID reader to seek a new card
        UART_PutArray(rfid_command_seek, sizeof(rfid_command_seek));
        #ifdef DEBUG
        kb_puts("Seeking for tag...\n");
        #endif
        // process messages until we get a serial number
        rfid_packet_length = 0;
        while(1) {
            rfid_read_packet(rfid_packet_buffer, RFID_PACKET_LEN, &rfid_packet_length);
            uint8_t command = rfid_packet_buffer[0];
            uint8_t tag_type = rfid_packet_buffer[1];
            if(command == RFID_COMMAND_SEEK) {
                // got tag with 7 byte serial number
                if(tag_type == RFID_TAG_TYPE_ULTRALIGHT) {
                    snprintf(display_buffer, 1024, "%02X%02X%02X%02X%02X%02X%02X",
                             rfid_packet_buffer[2],
                             rfid_packet_buffer[3],
                             rfid_packet_buffer[4],
                             rfid_packet_buffer[5],
                             rfid_packet_buffer[6],
                             rfid_packet_buffer[7],
                             rfid_packet_buffer[8]);
                    kb_puts(display_buffer);
                    break;
                }
                // got tag with 4 byte serial number
                else if(tag_type == RFID_TAG_TYPE_1K || tag_type == RFID_TAG_TYPE_4K) {
                    snprintf(display_buffer, 1024, "%02X%02X%02X%02X",
                             rfid_packet_buffer[2],
                             rfid_packet_buffer[3],
                             rfid_packet_buffer[4],
                             rfid_packet_buffer[5]);
                    kb_puts(display_buffer);
                    break;
                }
                // some other response without tag. keep looking
            }
        }
        // pause for human response time. otherwise we will get continuous output
        CyDelay(500);
    } // main loop
}


/* [] END OF FILE */
