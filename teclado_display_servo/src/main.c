/*
 * main.c
 *
 * Created: Wed Oct 17 19:33:59 UTC 2018
 * Author: ariel
*/

#include "lcd.h"
#include "matrix_keypad.h"
#include "servo.h"

static enum {
	READ_0,
	READ_1,
	READ_2,
	READ_CMD,
	MOTOR,
	CLEAR
} state;

static union {
	uint16_t total : 12;
	struct
	{
		uint8_t digit_0 : 4;
		uint8_t digit_1 : 4;
		uint8_t digit_2 : 4;
	} parts;
} buffer;

#define DIGIT_0 buffer.parts.digit_0
#define DIGIT_1 buffer.parts.digit_1
#define DIGIT_2 buffer.parts.digit_2
#define BUFFER buffer.total

#define LED_GPIO GPIO_B
#define LED_PIN PINB0

int main(void)
{
	// Program initialization
	inic_LCD_4bits();
	servo_init();
	matrix_keypad_init();

	uint8_t hold, angle = 0;
	BUFFER = 0xFFF;
	state = CLEAR;

	while (1)
	{
		switch (state)
		{

		case MOTOR:
			if (DIGIT_0 != 0xF)
				angle = DIGIT_0;
			if (DIGIT_1 != 0xF)
			{
				angle *= 10;
				angle += DIGIT_1;
			}
			if (DIGIT_2 != 0xF)
			{
				angle *= 10;
				angle += DIGIT_2;
			}
			servo_set_angle(angle);
			state = CLEAR;

		case CLEAR:
			cmd_LCD(1, 0);
			BUFFER = 0xFFF;
			state = READ_0;

		case READ_0:
			hold = matrix_keypad_read();
			if (hold == '#')
				state = MOTOR;
			else if (hold == '*')
				state = CLEAR;
			else if (hold >= '0' && hold <= '9')
			{
				cmd_LCD(1, 0);
				cmd_LCD(hold, 1);
				DIGIT_0 = (hold - '0');
				if (hold != '0')
					state = READ_1;
			}
			break;

		case READ_1:
			hold = matrix_keypad_read();
			if (hold == '#')
				state = MOTOR;
			else if (hold == '*')
				state = CLEAR;
			else if (hold >= '0' && hold <= '9')
			{
				cmd_LCD(hold, 1);
				DIGIT_1 = (hold - 0);
				state = READ_2;
			}
			break;

		case READ_2:
			hold = matrix_keypad_read();
			if (hold == '#')
				state = MOTOR;
			else if (hold == '*')
				state = CLEAR;
			else if ((DIGIT_0 == 1 && DIGIT_1 < 8) || (DIGIT_0 == 1 && DIGIT_1 == 8 && hold == '0'))
			{
				cmd_LCD(hold, 1);
				DIGIT_2 = (hold - 0);
				state = READ_CMD;
			}
			break;

		case READ_CMD:
			hold = matrix_keypad_read();
			if (hold == '#')
				state = MOTOR;
			else if (hold == '*')
				state = CLEAR;
			break;
		}
		_delay_ms(20);
	}

	// Should not reach
	return 0;
}
