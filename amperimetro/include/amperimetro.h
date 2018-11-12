/*
* amperimetro.h
*
* Created: 12/11/2018 10:53:50
*  Author: Ariel
*/

#ifndef AMPERIMETRO_H_
#define AMPERIMETRO_H_

#include <stdint.h>

typedef struct
{
	struct
	{
		uint8_t valor : 7;
		uint8_t is10por100 : 1;
	} escala;
	uint8_t corrente_inteiro;
	uint8_t corrente_decimal;
} amperimetro_t;

void amperimetro_inic(void);
void amperimetro_muda_escala(void);

amperimetro_t amperimetro_update(void);

#endif /* AMPERIMETRO_H_ */