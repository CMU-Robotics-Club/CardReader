#ifndef _PS2_H_
#define _PS2_H_

#include <Arduino.h>

/*
 * Converts a scanCode to it's numeric character.
 * If the scanCode does not represent a numeric character
 * returns ' '.
 */
char scanCodeToNumeric(uint8_t scanCode);

#endif
