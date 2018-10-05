#include <avr/interrupt.h>

#include "display_7seg_mux.h"
#include "display_7seg.h"

#include "avr_gpio.h"
#include "avr_timer.h"

display_7seg_t displays[DISPLAY_7SEG_MUX_N_DISPLAYS];
GPIOx_Type *SEGgpio;
GPIOx_Type *MUXgpio;
uint8_t muxMask;

void display_7seg_mux_init(GPIOx_Type *segments, GPIOx_Type *mux, uint8_t isCC)
{
    isCC = isCC ? 1 : 0;
    segments->DDR |= 0xEF;
    muxMask = (1 << DISPLAY_7SEG_MUX_N_DISPLAYS) - 1;
    mux->DDR = muxMask;
    muxMask ^= (isCC - 1);

    SEGgpio = segments;
    MUXgpio = mux;

    for (uint8_t i = 0; i < DISPLAY_7SEG_MUX_N_DISPLAYS; i++)
    {
        displays[i] = display_7seg_create(isCC);
    }

    TIMER_0->OCRA = 155;
    TIMER_0->TCCRB |= 0x04;
    TIMER_IRQS->TC0.BITS.OCIEA = 1;
}

void display_7seg_mux_set(uint8_t dispNum, uint8_t val)
{
    if (dispNum <= DISPLAY_7SEG_MUX_N_DISPLAYS)
    {
        display_7seg_set(val, &displays[dispNum]);
    }
}

void display_7seg_mux_set_all(uint8_t *vector)
{
    for (uint8_t i = 0; i < DISPLAY_7SEG_MUX_N_DISPLAYS; i++)
    {
        display_7seg_mux_set(i, vector[i]);
    }
}

ISR(TIMER0_COMPA_vect)
{
    static uint8_t i = DISPLAY_7SEG_MUX_N_DISPLAYS;
    if (++i >= DISPLAY_7SEG_MUX_N_DISPLAYS)
        i = 0;
    MUXgpio->PORT = muxMask;
    display_7seg_write(displays[i], SEGgpio);
    GPIO_CplBit(MUXgpio, i);
}