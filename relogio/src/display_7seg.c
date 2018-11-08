#include "display_7seg.h"

#include <stdint.h>

#include <avr/pgmspace.h>
static const uint8_t convTable[] PROGMEM = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xE7, 0xF7, 0xFC, 0xB9, 0xDE, 0xF9, 0xF1};

void display_7seg_set(uint8_t value, display_7seg_t *d)
{
    display_7seg_off(d);
    (*d) ^= pgm_read_byte(&(convTable[value & 0x0F])) << 1;
}


display_7seg_t display_7seg_create(uint8_t isCommonCathode)
{
    if (isCommonCathode)
        return 0x01;
    else
        return 0x00;
}

void display_7seg_write(display_7seg_t d, GPIOx_Type *gpio)
{
    gpio->PORT = d >> 1;
}

void display_7seg_off(display_7seg_t *d)
{
    (*d) = ((*d) & 0x01) - 0x01;
    CPL_BIT((*d), 0);
}