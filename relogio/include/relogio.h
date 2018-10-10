#ifndef RELOGIO_H_INCLUDED
#define RELOGIO_H_INCLUDED

#include <stdint.h>
#include "avr_gpio.h"

void relogio_init(GPIOx_Type *segmentsGPIO, GPIOx_Type *mux_GPIO);

uint8_t relogio_add_sec();
uint8_t relogio_add_min();
uint8_t relogio_add_hor();
void relogio_add();

uint8_t change_mode();
uint8_t get_mode();

#endif // RELOGIO_H_INCLUDED
