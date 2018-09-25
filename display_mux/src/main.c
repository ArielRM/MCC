#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "avr_gpio.h"
#include "bits.h"

#define COM_ANODO
#ifdef COM_ANODO
const uint8_t convTable[] PROGMEM = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02,
		0x78, 0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};
#endif

typedef struct display_mux_struct {
    uint8_t nDisp;
    uint8_t mask;
    GPIOx_Type *DispPort;
    GPIOx_Type *MuxPort;
} display_mux_t;

display_mux_t display_mux;

void display_mux_init(GPIOx_Type *DispPort, GPIOx_Type *MuxPort, uint8_t nDisp /*!< max 4 */)
{
    uint8_t mask = (2 << nDisp) - 1;

    DispPort->DDR = 0xFF;
    MuxPort->DDR = mask;

    display_mux.DispPort = DispPort;
    display_mux.MuxPort = MuxPort;
    display_mux.nDisp = nDisp;
    display_mux.mask = ~mask;
}

void display_mux_write(uint16_t n)
{
    // for (uint8_t i = display_mux.nDisp - 1; i >= 0; i--)
    // {
    //     uint8_t digit = pgm_read_byte(&convTable[n & 0x000F]);

    //     (display_mux.DispPort)->PORT = digit;

    //     GPIO_SetBit(display_mux.MuxPort, i);

    //     n = n >> 4; 
    //     _delay_ms(10);

    //     (display_mux.MuxPort)->PORT &= display_mux.mask;
    // }

    for (uint8_t i = 0; i < display_mux.nDisp; i++)
    {
        uint8_t digit = pgm_read_byte(&convTable[n & 0x000F]);

        (display_mux.DispPort)->PORT = digit;

        GPIO_SetBit(display_mux.MuxPort, (display_mux.nDisp - 1 - i));

        n = n >> 4; 
        _delay_ms(10);

        (display_mux.MuxPort)->PORT &= display_mux.mask;
    }
}

int main(void)
{
    display_mux_init(GPIO_D, GPIO_B, 4);

    CLR_BIT(DDRC, PC0);
    SET_BIT(PORTC, PC0);

    uint16_t i = 0;
    while (1)
    {
        while (PINC & (1 << PC0))
        {
            // Espera pressionar
            display_mux_write(i);
        }
        while (!(PINC & (1 << PC0)))
        {
            // Espera Soltar
            display_mux_write(i);
        }
        i++;
    }
}