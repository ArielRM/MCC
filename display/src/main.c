/*
 * main.c
 *
 * Created: Thu Oct  4 19:45:07 UTC 2018
 * Author: ariel
*/

#include <avr/io.h>
#include <util/delay.h>
#include "avr_gpio.h"

#include "display.h"

int main(void)
{
	display_t d = display_init(0, GPIO_B);
	while (1)
	{
	}
	// Should not reach
	return 0;
}
