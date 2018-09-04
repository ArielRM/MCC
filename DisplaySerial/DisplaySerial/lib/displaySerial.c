/*
 * displaySerial.c
 *
 * Created: 03/09/2018 10:27:27
 *  Author: Ariel
 */ 

#include "displaySerial.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "avr_gpio.h"
#include "bits.h"

const uint8_t convTable[] PROGMEM = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02,
0x78, 0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};

#define BCD2SEG(bcd) pgm_read_byte(&convTable[(bcd)])

#define PULSO_CLOCK() GPIO_SetBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_CLOCK);_delay_us(10);GPIO_ClrBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_CLOCK)

#define PULSO_STROBE() GPIO_SetBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_STROBE);_delay_us(10);GPIO_ClrBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_STROBE)

void displaySerial_init()
{
	SET_BIT(DISPLAYSERIAL_PORT->DDR, DISPLAYSERIAL_DATA);
	SET_BIT(DISPLAYSERIAL_PORT->DDR, DISPLAYSERIAL_CLOCK);
	SET_BIT(DISPLAYSERIAL_PORT->DDR, DISPLAYSERIAL_STROBE);
	
	GPIO_ClrBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_DATA);
	GPIO_ClrBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_CLOCK);
	GPIO_ClrBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_STROBE);
}

void displaySerial_write(uint8_t val)
{
	for (uint8_t j = 2; j > 0; --j)
	{
		uint8_t digit = BCD2SEG(val & 0x0F);
		for (uint8_t k = 8; k > 0; --k)
		{
			if (TST_BIT(digit, (k-1)))
			{
				GPIO_SetBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_DATA);
			}
			else
			{
				GPIO_ClrBit(DISPLAYSERIAL_PORT, DISPLAYSERIAL_DATA);
			}
			_delay_us(10);
			PULSO_CLOCK();
		}
		val >>= 4;
	}
	_delay_us(10);
	PULSO_STROBE();
}