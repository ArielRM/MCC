#ifndef DISPLAY_7SEG_H_INCLUDED
#define DISPLAY_7SEG_H_INCLUDED
#include <stdint.h>
#include "bits.h"
#include "avr_gpio.h"

#define display_7seg_CA (0)
#define display_7seg_CC (1)

typedef uint8_t display_7seg_t;
/*
struct {
    g : 1;
    f : 1;
    e : 1;
    d : 1;
    c : 1;
    b : 1;
    a : 1;
    
    isCommonCathode : 1;
}
*/

static inline display_7seg_t display_7seg_create(uint8_t isCommonCathode)
{
    return isCommonCathode ? 0x01 : 0x00;
}

static inline void display_7seg_write(display_7seg_t d, GPIOx_Type *gpio)
{
    gpio->PORT = d >> 1;
}

static inline void display_7seg_off(display_7seg_t *d)
{
    (*d) = ((*d) & 0x01) - 0x01;
    CPL_BIT((*d), 0);
}

void display_7seg_set(uint8_t value, display_7seg_t *d);

#endif //DISPLAY_7SEG_H_INCLUDED
