#include "display_7seg.h"

#include <stdint.h>

#include <avr/pgmspace.h>
const uint8_t convTable[] PROGMEM = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xE7, 0xF7, 0xFC, 0xB9, 0xDE, 0xF9, 0xF1};

void display_7seg_set(uint8_t value, display_7seg_t * d)
{
    display_7seg_off(d);
    (*d) ^= pgm_read_byte(&(convTable[value & 0x0F])) << 1;   
    return d;
}