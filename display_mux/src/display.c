#include <avr/pgmspace.h>
#include <stdint.h>

#include "avr_gpio.h"

#include "display.h"
/*
    seg 7   0-6
    dot 1   7
    on  1   8
    caC 1   9
    gpi 16  10
*/

struct display
{
    union {
        uint8_t FULL;
        struct
        {
            uint8_t a2f : 7;
            uint8_t dot : 1;
        } parts;
    } segments;
    uint8_t on : 1;
    uint8_t cc : 1;
    GPIOx_Type *GPIO;
    uint8_t reserved : 6;
};

const uint8_t convTable[] PROGMEM = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xE7, 0xF7, 0xFC, 0xB9, 0xDE, 0xF9, 0xF1};

display_t display_init(uint8_t isCC, GPIOx_Type *gpio)
{
    gpio->DDR = 0xFF;

    display_t d = 0;
    ((struct display *)&d)->GPIO = gpio;
    ((struct display *)&d)->cc = isCC;
    return d;
}

void display_set_value(display_t *d, uint8_t val)
{
    ((struct display *)d)->segments.parts.a2f = pgm_read_byte(&(convTable[val & 0x0F]));
}

void display_set_dot(display_t *d, uint8_t boolean)
{
    if (boolean)
        ((struct display *)d)->segments.parts.dot = 1;
    else
        ((struct display *)d)->segments.parts.dot = 0;
}

void display_set_on(display_t *d, uint8_t boolean)
{
    if (boolean)
        ((struct display *)d)->on = 1;
    else
        ((struct display *)d)->on = 0;
}

void display_flush(display_t *d)
{
    uint8_t hold;
    if (((struct display *)d)->cc)
    {
        if (((struct display *)d)->on)
            hold = ((struct display *)d)->segments.FULL;
        else
            hold = 0x00;
    }
    else
    {
        if (((struct display *)d)->on)
            hold = ~(((struct display *)d)->segments.FULL);
        else
            hold = 0xFF;
    }
    ((struct display *)d)->GPIO->PORT = hold;
}
