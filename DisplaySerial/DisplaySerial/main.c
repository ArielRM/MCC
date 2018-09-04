/*
 * DisplaySerial.c
 *
 * Created: 31/08/2018 17:25:40
 * Author : Ariel
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>

#include "lib/displaySerial.h"
#include "lib/avr_gpio.h"
#include "lib/bits.h"

#define BOTAO_PORT GPIO_D
#define BOTAO_PIN 2

int main(void)
{
	displaySerial_init();
	uint8_t i = 0;
	displaySerial_write(i);
	
	SET_BIT(BOTAO_PORT->DDR, BOTAO_PIN);
	GPIO_SetBit(BOTAO_PORT, BOTAO_PIN);
		
	while(1)
	{
		// Debounce simples
		while (GPIO_PinTstBit(BOTAO_PORT, BOTAO_PIN))
		{
			// Enquanto botão solto
		}
		
		displaySerial_write(++i);
		
		while (!GPIO_PinTstBit(GPIO_D, BOTAO_PIN))
		{
			// Enquanto botão pressionado
		}
		_delay_ms(3);
	}
}

