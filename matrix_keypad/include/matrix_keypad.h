#ifndef MATRIX_KEYPAD_H_INCLUDED
#define MATRIX_KEYPAD_H_INCLUDED

#include <avr/io.h>
#include "avr_gpio.h"

#define MKP_GPIO_A	(GPIO_B)
#define MKP_PIN_A	(PINB0)

#define MKP_GPIO_B	(GPIO_B)
#define MKP_PIN_B	(PINB1)

#define MKP_GPIO_C	(GPIO_B)
#define MKP_PIN_C	(PINB2)

#define MKP_GPIO_D	(GPIO_B)
#define MKP_PIN_D	(PINB3)

#define MKP_GPIO_1	(GPIO_B)
#define MKP_PIN_1	(PINB4)

#define MKP_GPIO_2	(GPIO_B)
#define MKP_PIN_2	(PINB5)

#define MKP_GPIO_3	(GPIO_B)
#define MKP_PIN_3	(PINB7)

void matrix_keypad_init();
uint8_t matric_keypad_read();

#endif //MATRIX_KEYPAD_H_INCLUDED