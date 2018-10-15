#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <stdint.h>

typedef uint32_t display_t;

display_t display_init(uint8_t isCC, GPIOx_Type *gpio);
void display_set_value(display_t *d, uint8_t val);
void display_set_dot(display_t *d, uint8_t boolean);
void display_set_on(display_t *d, uint8_t boolean);
void display_flush(display_t *d);

#endif // DISPLAY_H_INCLUDED
