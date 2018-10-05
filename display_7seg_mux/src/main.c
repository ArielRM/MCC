/*
 * main.c
 *
 * Created: Fri Oct  5 14:58:35 UTC 2018
 * Author: ariel
*/

#include <util/delay.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include "display_7seg_mux.h"

union {
	uint8_t vector[DISPLAY_7SEG_MUX_N_DISPLAYS];
	struct {
		uint8_t HOR_H;
		uint8_t HOR_L;
		uint8_t MIN_H;
		uint8_t MIN_L;
		uint8_t SEG_H;
		uint8_t SEG_L;
	} TIME;
} CLOCK;

int main(void)
{
	// Program initialization
	display_7seg_mux_init(GPIO_D, GPIO_B, 1);
	sei();
	while(1)
	{
		_delay_ms(1000);
		CLOCK.TIME.SEG_L++;
		if (CLOCK.TIME.SEG_L == 10)
		{
			CLOCK.TIME.SEG_L = 0;
			CLOCK.TIME.SEG_H++;
			if (CLOCK.TIME.SEG_H == 6)
			{
				CLOCK.TIME.SEG_H = 0;
				CLOCK.TIME.MIN_L++;
				if (CLOCK.TIME.MIN_L == 10)
				{
					CLOCK.TIME.MIN_L = 0;
					CLOCK.TIME.MIN_H++;
					if (CLOCK.TIME.MIN_H == 6)
					{
						CLOCK.TIME.MIN_H = 0;
					}
				}
			}
		}
		display_7seg_mux_set_all(CLOCK.vector);	
	}
	// Should not reach
	return 0;
}
