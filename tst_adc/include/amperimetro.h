#ifndef AMPERIMETRO_H_INCLUDED
#define AMPERIMETRO_H_INCLUDED

#include <stdint.h>

typedef struct
{
	uint8_t escala : 7;
	uint8_t isFundo : 1;
	uint16_t corrente_vezes_800;
} amperimetro_t;

void amperimetro_init();
void amperimetro_muda_escala();
amperimetro_t *amperimetro();

#endif //AMPERIMETRO_H_INCLUDED