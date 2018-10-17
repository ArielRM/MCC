/*
 * main.c
 *
 * Created: Wed Oct 17 19:33:59 UTC 2018
 * Author: ariel
*/

#include <avr/io.h>
#include "lcd.h"
#include "matrix_keypad.h"
#include "servo.h"

int main(void)
{
	// Program initialization
	inic_LCD_4bits();
	servo_init();
	matrix_keypad_init();
	
	while (1)
	{
		
	}
	// Should not reach
	return 0;
}
