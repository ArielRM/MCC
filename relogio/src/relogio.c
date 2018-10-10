#include "relogio.h"

#include <avr/interrupt.h>

#include "display_7seg_mux.h"

#include "avr_gpio.h"
#include "avr_timer.h"

#include "bits.h"

uint8_t relogio[6] = {0};
volatile uint8_t mode = 0;

void relogio_init(GPIOx_Type *segmentsGPIO, GPIOx_Type *mux_GPIO)
{
    display_7seg_mux_init(segmentsGPIO, mux_GPIO, 0);

    TIMER_1->TCCRB |= SET(WGM12) | 0x04; // CTCA com Prescale em 256;
    TIMER_1->OCRA = 15625;               // TOP <= 15625

    /*
        16MHz/256 = 625000Hz
        1/62500Hz = 16us
        16us * 15625 = 250ms
    */

    TIMER_IRQS->TC1.BITS.OCIEA = 1;

    display_7seg_mux_set_all(relogio);
}

uint8_t relogio_add_sec()
{
    relogio[0] += 1;
    if (relogio[0] == 10)
    {
        relogio[0] = 0;
        relogio[1] += 1;
        if (relogio[1] == 6)
        {
            relogio[1] = 0;
            return 1;
        }
    }
    return 0;
}

uint8_t relogio_add_min()
{
    relogio[2] += 1;
    if (relogio[2] == 10)
    {
        relogio[2] = 0;
        relogio[3] += 1;
        if (relogio[3] == 6)
        {
            relogio[3] = 0;
            return 1;
        }
    }
    return 0;
}

uint8_t relogio_add_hor()
{
    relogio[4] += 1;
    if (relogio[4] == 10)
    {
        relogio[4] = 0;
        relogio[5] += 1;
    }
    else if (relogio[5] == 2 && relogio[4] == 4)
    {
        relogio[5] = 0;
        relogio[4] = 0;
        return 1;
    }
    return 0;
}

void relogio_add()
{
    switch (mode)
    {
    case 1:
        relogio_add_sec();
        break;
    case 2:
        relogio_add_min();
        break;
    case 3:
        relogio_add_hor();
        break;
    default:
        if (relogio_add_sec())
            if (relogio_add_min())
                relogio_add_hor();
    }
}

uint8_t change_mode()
{
    mode++;
    if (mode == 4)
    {
        mode = 0;
    }
    return mode;
}

uint8_t get_mode()
{
    return mode;
}

ISR(TIMER1_COMPA_vect)
{
    static uint8_t quart_sec = 0;
    if (quart_sec == 4)
        quart_sec = 0;
    if (quart_sec == 3 || quart_sec == 0)
    {
        if (mode == 0 && quart_sec == 3)
        {
            relogio_add();
        }
        display_7seg_mux_set_all(relogio);
    }
    else
    {
        switch (mode)
        {
        case 1:
            display_7seg_mux_off(0);
            display_7seg_mux_off(1);
            break;
        case 2:
            display_7seg_mux_off(2);
            display_7seg_mux_off(3);
            break;
        case 3:
            display_7seg_mux_off(4);
            display_7seg_mux_off(5);
        }
    }
    quart_sec += 1;
}