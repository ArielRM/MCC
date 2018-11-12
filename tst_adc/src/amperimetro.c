#include "amperimetro.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

static volatile amperimetro_t amp;
static volatile uint16_t buffer[8] = {0};

static volatile enum {
	A10,
	A20,
	A40,
	A50,
} amperimetro_estado;

static void f_A10()
{
	amp.escala = 10;
	amp.corrente_vezes_800 *= 10;
	if (amp.corrente_vezes_800 >= 800)
		amp.isFundo = 1;
	else
		amp.isFundo = 0;
	ADMUX &= ~0x03;
}

static void f_A20()
{
	amp.escala = 20;
	amp.corrente_vezes_800 *= 20;
	if (amp.corrente_vezes_800 >= 1600)
		amp.isFundo = 1;
	else
		amp.isFundo = 0;
	ADMUX &= ~0x03;
	ADMUX |= 1;
}

static void f_A40()
{
	amp.escala = 40;
	amp.corrente_vezes_800 *= 40;
	if (amp.corrente_vezes_800 >= 3200)
		amp.isFundo = 1;
	else
		amp.isFundo = 0;
	ADMUX &= ~0x03;
	ADMUX |= 2;
}

static void f_A50()
{
	amp.escala = 50;
	amp.corrente_vezes_800 *= 50;
	if (amp.corrente_vezes_800 >= 4000)
		amp.isFundo = 1;
	else
		amp.isFundo = 0;
	ADMUX &= ~0x03;
	ADMUX |= 3;
}

static void (*fsm[])(void) = {f_A10, f_A20, f_A40, f_A50};

void amperimetro_init()
{
	ADMUX &= (0xF8 | 0); // MUX 0

	ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin

	ADCSRA |= (1 << ADEN) |								   // ADC Enable
			  (1 << ADATE) |							   // ADC Auto Trigger
			  (1 << ADIE) |								   // AD IRQ ENABLE
			  (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | // Prescaler = 128
			  (1 << ADSC);								   // ADC Start conversion

	// Desabilita entradas digitais PC0..PC3
	DIDR0 |= (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D) | (1 << ADC3D);

	// Sinal logico alto nas entradas digitais desabilitadas
	PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);

	amperimetro_estado = A10;

	amp.corrente_vezes_800 = 0;
}

void amperimetro_muda_escala()
{
	amp.corrente_vezes_800 = 0;
	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;
	buffer[4] = 0;
	buffer[5] = 0;
	buffer[6] = 0;
	buffer[7] = 0;
	switch (amperimetro_estado)
	{
	case A10:
		amperimetro_estado = A20;
		break;
	case A20:
		amperimetro_estado = A40;
		break;
	case A40:
		amperimetro_estado = A50;
		break;
	case A50:
		amperimetro_estado = A10;
		break;
	}
}

amperimetro_t *amperimetro()
{
	fsm[amperimetro_estado]();
	return &amp;
}

ISR(ADC_vect)
{
	static uint8_t i = 0;
	if (++i == 8)
		i = 0;
	amp.corrente_vezes_800 -= buffer[i];
	buffer[i] = (uint32_t)(ADC * 100) >> 10;
	amp.corrente_vezes_800 += buffer[i];
}