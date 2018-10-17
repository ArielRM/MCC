#ifndef MATRIX_KEYPAD_H_INCLUDED
#define MATRIX_KEYPAD_H_INCLUDED

#include <avr/io.h>
#include "avr_gpio.h"

#define MKP_GPIO_A	(GPIO_D)
#define MKP_PIN_A	(PIND0)

#define MKP_GPIO_B	(GPIO_D)
#define MKP_PIN_B	(PIND1)

#define MKP_GPIO_C	(GPIO_D)
#define MKP_PIN_C	(PIND2)

#define MKP_GPIO_D	(GPIO_D)
#define MKP_PIN_D	(PIND3)

#define MKP_GPIO_1	(GPIO_D)
#define MKP_PIN_1	(PIND4)

#define MKP_GPIO_2	(GPIO_D)
#define MKP_PIN_2	(PIND5)

#define MKP_GPIO_3	(GPIO_D)
#define MKP_PIN_3	(PIND6)

void matrix_keypad_init();
uint8_t matrix_keypad_read();

#endif //MATRIX_KEYPAD_H_INCLUDED