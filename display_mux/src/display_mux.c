#include <avr/interrupt.h>

#include "avr_gpio.h"
#include "avr_timer.h"
#include "display_mux.h"
#include "display.h"

static volatile display_t displays[DISPLAY_MUX_N];
static GPIOx_Type *muxGPIO;

ISR(TIMER0_COMPA_vect)
{

}

void display_mux_init(GPIOx_Type *display, uint8_t isCC, GPIOx_Type *mux)
{
    mux->DDR = (1 << DISPLAY_MUX_N) - 1;
    muxGPIO = mux;
    for (uint8_t i; i < DISPLAY_MUX_N; i++)
    {
        displays[i] = display_init(isCC, display);
    }
    TIMER_0->TCCRA |= 4 | (1 << WGM02);
    TIMER_0->OCRA = 624;
    TIMER_IRQS->TC0.BITS.OCIEA = 1;
}