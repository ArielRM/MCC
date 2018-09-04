/*
 * displaySerial.h
 *
 * Created: 04/09/2018 10:19:59
 *  Author: Ariel
 */ 


#ifndef DISPLAYSERIAL_H_
#define DISPLAYSERIAL_H_

#include <stdint.h>
#include <avr/io.h>
#include "avr_gpio.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define DISPLAYSERIAL_PORT GPIO_B
#define DISPLAYSERIAL_DATA PORTB0
#define DISPLAYSERIAL_CLOCK PORTB1
#define DISPLAYSERIAL_STROBE PORTB2


void displaySerial_init();
void displaySerial_write(uint8_t val);


#endif /* DISPLAYSERIAL_H_ */