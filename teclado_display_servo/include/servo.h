#ifndef SERVO_H_INCLUDED
#define SERVO_H_INCLUDED
#include <stdint.h>

void servo_init();
void servo_set_angle(uint8_t angle);

#endif //SERVO_H_INCLUDED