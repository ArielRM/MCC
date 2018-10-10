/*
 * main.c
 *
 * Created: Tue Oct  9 12:52:24 UTC 2018
 * Author: ariel
*/

#include <avr/interrupt.h>

#include "avr_extirq.h"
#include "avr_gpio.h"
#include "bits.h"

#include "relogio.h"

#define DISPLAY_GPIO GPIO_D
#define MUX_GPIO GPIO_B

#define BT_GPIO GPIO_C
#define SET_BT PCINT8
#define ADD_BT PCINT9

void bts_init()
{
	SET_BIT(BT_GPIO->DDR, SET_BT);
	SET_BIT(BT_GPIO->DDR, ADD_BT);

	GPIO_SetBit(BT_GPIO, SET_BT);
	GPIO_SetBit(BT_GPIO, ADD_BT);

	EXT_IRQ->PC_INT.BITS.PCIE_1 = 1;
	EXT_IRQ_PCINT_MASK->PCMASK1.BITS.PCINT_8 = 1;
}

int main(void)
{
	// Program initialization
	relogio_init(DISPLAY_GPIO, MUX_GPIO);
	bts_init();

	sei();
	while (1)
	{
	}
	// Should not reach
	return 0;
}

ISR(PCINT1_vect)
{
	if (!GPIO_PinTstBit(GPIO_C, PCINT8))
	{
		if (change_mode() == 0)
		{
			EXT_IRQ_PCINT_MASK->PCMASK1.BITS.PCINT_9 = 0;
		}
		else
		{
			EXT_IRQ_PCINT_MASK->PCMASK1.BITS.PCINT_9 = 1;
		}
	}
	else if (!GPIO_PinTstBit(GPIO_C, PCINT9))
	{
		relogio_add();
	}
}