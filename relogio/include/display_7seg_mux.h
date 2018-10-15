#ifndef DISPLAY_7SEG_MUX_H_INCLUDED
#define DISPLAY_7SEG_MUX_H_INCLUDED

#include <stdint.h>
#include "avr_gpio.h"

#define DISPLAY_7SEG_MUX_N_DISPLAYS 6

void display_7seg_mux_init(GPIOx_Type *segments, GPIOx_Type *mux, uint8_t isCC);
void display_7seg_mux_set(uint8_t dispNum, uint8_t val);
void display_7seg_mux_set_all(uint8_t *vector);
void display_7seg_mux_off(uint8_t dispNum);

#endif // DISPLAY_7SEG_MUX_H_INCLUDED
