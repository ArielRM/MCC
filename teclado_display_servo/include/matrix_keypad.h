#ifndef MATRIX_KEYPAD_H_INCLUDED
#define MATRIX_KEYPAD_H_INCLUDED

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "avr_gpio.h"

#define MKP_GPIO_A	(GPIO_D)
#define MKP_PIN_A	(PIND7)

#define MKP_GPIO_B	(GPIO_D)
#define MKP_PIN_B	(PIND6)

#define MKP_GPIO_C	(GPIO_D)
#define MKP_PIN_C	(PIND5)

#define MKP_GPIO_D	(GPIO_D)
#define MKP_PIN_D	(PIND4)

#define MKP_GPIO_1	(GPIO_B)
#define MKP_PIN_1	(PINB5)

#define MKP_GPIO_2	(GPIO_B)
#define MKP_PIN_2	(PINB4)

#define MKP_GPIO_3	(GPIO_B)
#define MKP_PIN_3	(PINB3)

void matrix_keypad_init();
uint8_t matrix_keypad_read();

#endif //MATRIX_KEYPAD_H_INCLUDED