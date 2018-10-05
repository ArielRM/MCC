/*
 * main.c
 *
 * Created: Wed Oct  3 20:50:02 UTC 2018
 * Author: ariel
*/

#include <avr/io.h>

#define DATA_PORT PORTB
#define DATA_PIN PB0
#define DATA_DDR DDRB

void sendLow()
{
	DATA_PORT = 1;
	asm volatile("lpm\n\t"); // 375 ns
	DATA_PORT = 0;
	asm volatile("lpm\n\t"
				 "lpm\n\t"
				 "nop\n\t"
				 "nop\n\t"); // 687,5 ns
}

void sendHigh()
{
	DATA_PORT = 1;
	asm volatile("ldi  r18, 3\n\t"
				 "L2: dec  r18\n\t"
				 "brne L2\n\t"
				 "nop\n\t"); // 812,5 ns

	DATA_PORT = 0;
	asm volatile("lpm\n\t"
				 "lpm\n\t"
				 "nop\n\t"); // 625 ns
}

void reset()
{
	DATA_PORT = 0;
	asm volatile("ldi  r18, 2\n\t"
				 "ldi r19, 9\n\t "
				 "L1: dec  r19\n\t"
				 "brne L1\n\t"
				 "dec  r18\n\t"
				 "brne L1\n\t"); // 50us
}

int main(void)
{
	// Program initialization
	DATA_DDR |= 1;

	reset();

	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();

	sendLow();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();

	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendLow();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	sendHigh();
	while (1)
	{
		// Program loop
	}
	// Should not reach
	return 0;
}
