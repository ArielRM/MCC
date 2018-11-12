#include <util/delay.h>
#include <avr/interrupt.h>

#include "amperimetro.h"

#ifdef LCD
#include "lcd.h"
#else
#include "avr_usart.h"
#endif

static inline void botao_PCINT7_init()
{
	DDRB &= ~(1 << PCINT7);
	PORTB |= (1 << PCINT7);
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT7);
}

ISR(PCINT0_vect)
{
	if (!(PINB & (1 << PCINT7)))
		amperimetro_muda_escala();
}

int main()
{
	/* Inicializa o amperimetro Escala: 10 */
	amperimetro_init();

	/* Configura PCINT7 (PB7) para alterar escala */
	botao_PCINT7_init();

#ifdef LCD
	inic_LCD_4bits();
	/* Inicializa o lcd */
	FILE *f = inic_stream();
#endif //LCD

	USART_Init(B9600);
	FILE *f = get_usart_stream();

	/* Ativa todos IRQs */
	sei();

	while (1)
	{
		amperimetro_t *hold = amperimetro();
		uint16_t i = hold->corrente_vezes_800 >> 3;

#ifdef LCD
		cmd_LCD(0x01, 0);
		fprintf(f, "Escala: %d", hold->escala);
		cmd_LCD(0xC0, 0);
		fprintf(f, "%d", i);
#endif //LCD

		fprintf(f, "Escala: %d", hold->escala);
		fprintf(f, "\r");
		fprintf(f, "%d", i);
		if (hold->isFundo)
			fprintf(f, "*\r");
		else
			fprintf(f, "\r");

		_delay_ms(500);
	}
}
