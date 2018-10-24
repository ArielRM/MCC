/*
 * main.c
 *
 * Created: Wed Oct 24 17:41:49 UTC 2018
 * Author: ariel
*/

/*
 * main_an_comp.c
 *
 *  Created on: Oct 08, 2018
 *      Author: Renan Augusto Starke
 *      Instituto Federal de Santa Catarina
 */

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr_gpio.h"

#include "avr_adc.h"
#include "avr_extirq.h"
#include "bits.h"

#define DEBUG
#ifdef DEBUG
#include "avr_usart.h"
static FILE *_debug;
#define debug(str, ...) fprintf(_debug, str, ##__VA_ARGS__)
#define CLEAR_TERMINAL() fputc(12, _debug)
#else
#define debug(...)
#endif //DEBUG

static volatile uint16_t i_buffer[8] = {0};
static volatile uint8_t scale = 0;

void adc_init();

int main(void)
{
	// Program initialization

#ifdef DEBUG
	USART_Init(B9600);
	_debug = get_usart_stream();
#endif //DEBUG

	/* Inicializa o converor AD */
	adc_init();

	EXT_IRQ->PC_INT.BITS.PCIE_1 = 1;
	EXT_IRQ_PCINT_MASK->PCMASK1.BITS.PCINT_12 = 1;
	DDRC |= SET(PCINT12);
	PORTC |= SET(PCINT12);

	/* Ativa todos IRQs */
	sei();

	/* Exbibe no terminal o valor do AD.
	 * A conversão é realizada conforme o prescaler
	 */
	while (1)
	{
		uint16_t hold =
			i_buffer[0] +
			i_buffer[1] +
			i_buffer[2] +
			i_buffer[3] +
			i_buffer[4] +
			i_buffer[5] +
			i_buffer[6] +
			i_buffer[7];
		hold >>= 3;
		CLEAR_TERMINAL();
		debug("scale: %d\rvalue: %d\r", scale, hold);
		_delay_ms(500);
	}
}

void adc_init()
{

	/* Ref externa no pino AVCC com capacitor de 100n em VREF.
	 * Habiltiação apenas no Canal 0 */
	ADCS->AD_MUX = SET(REFS0);

	/* Habilita AD:
	 * Conversão contínua
	 * IRQ ativo
	 * Prescaler de 128 */
	ADCS->ADC_SRA = SET(ADEN) |							   //ADC Enable
					SET(ADSC) |							   // ADC Start conversion
					SET(ADATE) |						   // ADC Auto Trigger
					SET(ADPS0) | SET(ADPS1) | SET(ADPS2) | //ADPS[0..2] AD Prescaler selection
					SET(ADIE);							   //AD IRQ ENABLE

	/* Desabilita hardware digital de PC0 */
	ADCS->DIDr0.BITS.ADC0 = 1;
	ADCS->DIDr0.BITS.ADC1 = 1;
	ADCS->DIDr0.BITS.ADC2 = 1;
	ADCS->DIDr0.BITS.ADC3 = 1;
}

ISR(ADC_vect)
{
	static uint8_t i = 0;
	i_buffer[i++] = ADC;
	i &= 0x07;
}

ISR(PCINT1_vect)
{
	if (!GPIO_PinTstBit(GPIO_C, PCINT12))
	{
		scale++;
		scale &= 0x03;
		ADCS->AD_MUX &= ~0x03;
		ADCS->AD_MUX |= scale;
	}
}