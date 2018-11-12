#include <avr/io.h>
#include "display_7seg.h"

#include "avr_gpio.h"

int main(void)
{
	DDRD = 0xFF;
	display_7seg_t disp = display_7seg_create(display_7seg_CC);
	display_7seg_set(4, &disp);
	display_7seg_write(disp, GPIO_D);
	while (1)
	{

	}

	return 0;
}
