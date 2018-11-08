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

display_7seg_t display_7seg_create(uint8_t isCommonCathode);
void display_7seg_write(display_7seg_t d, GPIOx_Type *gpio);
void display_7seg_off(display_7seg_t *d);
void display_7seg_set(uint8_t value, display_7seg_t *d);

#endif //DISPLAY_7SEG_H_INCLUDED
