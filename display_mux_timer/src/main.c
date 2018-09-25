/*
 * display_mux_timer.c
 *
 * Created: 25/09/2018 08:09:51
 *  Author: Aluno
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#include "avr_gpio.h"
#include "avr_extirq.h"
#include "avr_timer.h"
#include "bits.h"

#define CLOCK_PORT GPIO_B
#define CLOCK_PIN PB0

void timer1_hardware_init(){

	/* Acesso indireto por struct e bit field: com avr_timer.h */
	TIMER_1->TCCRA = 0;
	/* Modo CTC e prescaler 1024 */
	TIMER_1->TCCRB = SET(WGM12) |  SET(CS12) | SET(CS10);

	/* Definição do TOP */
	TIMER_1->OCRA = 7812;

	/* Habilitação da IRQ: capture pois o top é OCR1A */
	TIMER_IRQS->TC1.BITS.OCIEA = 1;
}

ISR(TIMER1_COMPA_vect){
	GPIO_CplBit(GPIO_B, CLOCK_PIN);
}

int main(void)
{
	CLOCK_PORT->DDR |= SET(CLOCK_PIN);
	
	timer1_hardware_init();
	
	sei();
    while(1)
    {
		
    }
}