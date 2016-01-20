#include "ps2.h"

char scanCodeToNumeric(uint8_t scanCode) {
  switch(scanCode) {
    case 0x45:
      return '0';
    case 0x16:
      return '1';
    case 0x1E:
      return '2';
    case 0x26:
      return '3';
    case 0x25:
      return '4';
    case 0x2E:
      return '5';
    case 0x36:
      return '6';
    case 0x3D:
      return '7';
    case 0x3E:
      return '8';
    case 0x46:
      return '9';
    default:
      // return space if not numeric
      return ' ';
  }
}
