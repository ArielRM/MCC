#include "matrix_keypad.h"

#include <avr/pgmspace.h>

#define CLR_GP(TYPE, PIN) ((MKP_GPIO_##PIN)->TYPE &= ~(1 << MKP_PIN_##PIN))
#define SET_GP(TYPE, PIN) ((MKP_GPIO_##PIN)->TYPE |= (1 << MKP_PIN_##PIN))
#define GET_GP(TYPE, PIN) ((MKP_GPIO_##PIN)->TYPE & (1 << MKP_PIN_##PIN))

//	 MATRIX[L][C]
static const uint8_t MATRIX[4][3] PROGMEM = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}};

void matrix_keypad_init()
{
	SET_GP(DDR, 1);
	SET_GP(DDR, 2);
	SET_GP(DDR, 3);

	CLR_GP(DDR, A);
	CLR_GP(DDR, B);
	CLR_GP(DDR, C);
	CLR_GP(DDR, D);

	SET_GP(PORT, 1);
	SET_GP(PORT, 2);
	SET_GP(PORT, 3);

	SET_GP(PORT, A);
	SET_GP(PORT, B);
	SET_GP(PORT, C);
	SET_GP(PORT, D);
}

uint8_t matric_keypad_read()
{
	uint8_t line, column = 0;
	while (GET_GP(PIN, A) && GET_GP(PIN, B) && GET_GP(PIN, C) && GET_GP(PIN, D))
	{
		switch (++column)
		{
		case 3:
			column = 0;
		case 0:
			SET_GP(PORT, 3);
			CLR_GP(PORT, 1);
			break;
		case 1:
			SET_GP(PORT, 1);
			CLR_GP(PORT, 2);
			break;
		case 2:
			SET_GP(PORT, 2);
			CLR_GP(PORT, 3);
		}
	}
	if (!GET_GP(PIN, A))
	{
		line = 0;
	}

	if (!GET_GP(PIN, B))
	{
		line = 1;
	}

	if (!GET_GP(PIN, C))
	{
		line = 2;
	}

	if (!GET_GP(PIN, D))
	{
		line = 3;
	}
	while (!(GET_GP(PIN, A) && GET_GP(PIN, B) && GET_GP(PIN, C) && GET_GP(PIN, D)))
	{

	}
	SET_GP(PORT, 1);
	SET_GP(PORT, 2);
	SET_GP(PORT, 3);
	return pgm_read_byte(&(MATRIX[line][column]));
}