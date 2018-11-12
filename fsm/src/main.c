/*
 * main.c
 *
 * Created: Wed Oct 31 14:37:33 UTC 2018
 * Author: ariel
*/

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#include "fsm.h"
#include "lcd.h"
#include "avr_extirq.h"

static fsm_t fsm;
static uint8_t scale;

void A10()
{
	scale = 10;
	escreve_LCD("Escala: 10A");
}

void A20()
{
	scale = 20;
	escreve_LCD("Escala: 20A");
}

void A40()
{
	scale = 40;
	escreve_LCD("Escala: 40A");
}

void A50()
{
	scale = 50;
	escreve_LCD("Escala: 50A");
}

uint8_t next_state(uint8_t curr)
{
	return (curr + 1) & 0x03;
}

int main(void)
{
	state_func_t states[4] = {A10, A20, A40, A50};

	fsm_init(&fsm, next_state, states);

	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);
	EXT_IRQ->INT0_INT1.BITS.ISC_01 = 1;
	EXT_IRQ_EI_MASK->EIMASK.BITS.INT_0 = 1;

	inic_LCD_4bits();

	sei();

	while (1)
	{
		cmd_LCD(0x02, 0);
		fsm_curr(&fsm);
		cmd_LCD(0xC0, 0);
		escreve_LCD("Valor: ");
	}
	// Should not reach
	return 0;
}

ISR(INT0_vect)
{
	fsm_next(&fsm);
}