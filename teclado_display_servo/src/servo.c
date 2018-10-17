#include "servo.h"

#include <avr/io.h>

void servo_init()
{
	DDRB |= (1 << PB1);
	/*			16MHz 
	fpwm = 	-------------	
			(2 * N * TOP)
	N = 8
	TOP = 0x4E20 = 20k
	fpwm = 50Hz		Tpwm = 20ms
	*/
	ICR1	= 20000;
	OCR1A	= 1000;

	TCCR1A |=
		(1 << WGM11) |  //PWM, Phase Correct TOP ICR1
		(1 << COM1A1);	//Set OC1A/OC1B on Compare Match when upcounting ...

	TCCR1B |=
		(1 << WGM13) | //PWM, Phase Correct TOP ICR1
		0x02;		   //Prescaler at 8
}

void servo_set_angle(uint8_t angle) 
{
	OCR1A = ((uint16_t) angle * 50) / 9 + 1000;
}
