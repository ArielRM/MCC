/*
 * main.c
 *
 * Created: Thu Oct  4 23:21:12 UTC 2018
 * Author: ariel
*/

#include <util/delay.h>
#include <avr/io.h>

#include "display_7seg.h"
#include "avr_gpio.h"

int main(void)
{
	DDRD = 0xFF;
	display_7seg_t d = display_7seg_create(display_7seg_CC);
	uint8_t i = 0;
	while (1)
	{
		display_7seg_set( i++, &d);
		display_7seg_write(d, GPIO_D);
		_delay_ms(1000);
		display_7seg_off(&d);
		display_7seg_write(d, GPIO_D);
		_delay_ms(1000);
	}
	// Should not reach
	return 0;
}
