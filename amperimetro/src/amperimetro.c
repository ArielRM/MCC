/*
* amperimetro.c
*
* Created: 12/11/2018 10:39:51
*  Author: Ariel
*/

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "amperimetro.h"

static uint8_t amperimetro_escala;
static volatile uint16_t amperimetro_valor;

static volatile uint16_t amperimetro_buffer[8];
static volatile uint8_t amperimetro_buffer_index;

static volatile enum {
	A10,
	A20,
	A40,
	A50
} amperimetro_state;

static void f_10A(void)
{
	amperimetro_escala = 10;
	ADMUX = (ADMUX & ~0x03) | 0x00;
}

static void f_20A(void)
{
	amperimetro_escala = 20;
	ADMUX = (ADMUX & ~0x03) | 0x01;
}

static void f_40A(void)
{
	amperimetro_escala = 40;
	ADMUX = (ADMUX & ~0x03) | 0x02;
}

static void f_50A(void)
{
	amperimetro_escala = 50;
	ADMUX = (ADMUX & ~0x03) | 0x03;
}

static void (*const amperimetro_fsm[])(void) = {f_10A, f_20A, f_40A, f_50A};

void amperimetro_inic(void)
{
	amperimetro_state = A10;
	amperimetro_valor = 0;
	amperimetro_buffer_index = 0;

	// Desabilita entradas digitais PC0..PC3
	DIDR0 |= (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D) | (1 << ADC3D);

	// Sinal logico alto nas entradas digitais desabilitadas
	PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);

	ADMUX &= (0xF8 | 0); // MUX 0

	ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin

	ADCSRA |= (1 << ADEN) |								   // ADC Enable
			  (1 << ADATE) |							   // ADC Auto Trigger
			  (1 << ADIE) |								   // AD IRQ ENABLE
			  (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | // Prescaler = 128
			  (1 << ADSC);								   // ADC Start conversion
}

void amperimetro_muda_escala(void)
{
	switch (amperimetro_state)
	{
	case A10:
		amperimetro_state = A20;
		break;
	case A20:
		amperimetro_state = A40;
		break;
	case A40:
		amperimetro_state = A50;
		break;
	case A50:
		amperimetro_state = A10;
		break;
	}
}

amperimetro_t amperimetro_update(void)
{
	amperimetro_fsm[amperimetro_state]();
	amperimetro_t hold;
	uint8_t _escala = amperimetro_escala;
	uint32_t _valor = amperimetro_valor;
	hold.escala.valor = _escala;
	_valor *= _escala * 100;
	_valor >>= 13;
	hold.corrente_decimal = _valor % 100;
	hold.corrente_inteiro = _valor / 100;

	if (_valor >= _escala * 90)
		hold.escala.is10por100 = 1;
	else
		hold.escala.is10por100 = 0;

	return hold;
}

ISR(ADC_vect)
{
	amperimetro_valor -= amperimetro_buffer[amperimetro_buffer_index];
	amperimetro_buffer[amperimetro_buffer_index] = ADC;
	amperimetro_valor += amperimetro_buffer[amperimetro_buffer_index];
	if (++amperimetro_buffer_index == 8)
		amperimetro_buffer_index = 0;
}