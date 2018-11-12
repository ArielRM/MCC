/*
* amperimetro2.c
*
* Created: 12/11/2018 10:39:33
* Author : Ariel
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "amperimetro.h"
#include "lcd.h"

static inline void bt_escala_init()
{
	DDRB &= ~(1 << PCINT7);
	PORTB |= (1 << PCINT7);
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT7);
}

int main(void)
{
	inic_LCD_4bits();
	FILE *f = inic_stream();

	amperimetro_inic();
	bt_escala_init();

	sei();

	while (1)
	{
		amperimetro_t hold = amperimetro_update();

		cmd_LCD(0x01, 0);
		fprintf(f, "Escala: %dA", hold.escala.valor);
		cmd_LCD(0xC0, 0);
		fprintf(f, "valor: %u,%u", hold.corrente_inteiro, hold.corrente_decimal);
		if (hold.escala.is10por100)
		{
			lcd_putchar('*', f);
		}
		_delay_ms(500);
	}
}

ISR(PCINT0_vect)
{
	if (!(PINB & (1 << PCINT7)))
	{
		amperimetro_muda_escala();
	}
}