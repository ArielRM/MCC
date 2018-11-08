#include "relogio.h"

#include <avr/interrupt.h>

#include "display_7seg_mux.h"

#include "avr_gpio.h"
#include "avr_timer.h"

#include "bits.h"

#define SEG_L 0
#define SEG_H 1

#define MIN_L 2
#define MIN_H 3

#define HOR_L 4
#define HOR_H 5

static uint8_t relogio[6] = {0};
static volatile uint8_t mode = 0;

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
    relogio[SEG_L] += 1;
    if (relogio[SEG_L] == 10)
    {
        relogio[SEG_L] = 0;
        relogio[SEG_H] += 1;
        if (relogio[SEG_H] == 6)
        {
            relogio[SEG_H] = 0;
            return 1;
        }
    }
    return 0;
}

uint8_t relogio_add_min()
{
    relogio[MIN_L] += 1;
    if (relogio[MIN_L] == 10)
    {
        relogio[MIN_L] = 0;
        relogio[MIN_H] += 1;
        if (relogio[MIN_H] == 6)
        {
            relogio[MIN_H] = 0;
            return 1;
        }
    }
    return 0;
}

uint8_t relogio_add_hor()
{
    relogio[HOR_L] += 1;
    if (relogio[HOR_L] == 10)
    {
        relogio[HOR_L] = 0;
        relogio[HOR_H] += 1;
    }
    else if (relogio[HOR_H] == 2 && relogio[HOR_L] == 4)
    {
        relogio[HOR_H] = 0;
        relogio[HOR_L] = 0;
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

uint8_t relogio_change_mode()
{
    mode++;
    if (mode == 4)
    {
        mode = 0;
    }
    return mode;
}

void relogio_clear_seconds()
{
    relogio[SEG_L] = 0;
    relogio[SEG_H] = 0;
    display_7seg_mux_set_all(relogio);
}

uint8_t relogio_get_mode()
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