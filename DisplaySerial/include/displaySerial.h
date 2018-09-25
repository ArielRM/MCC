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

void displaySerial_init(GPIOx_Type *port, uint8_t dataPin, uint8_t clockPin, uint8_t strobePin, uint8_t num);
void displaySerial_write(uint8_t val);

#endif /* DISPLAYSERIAL_H_ */