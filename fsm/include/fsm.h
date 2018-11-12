#ifndef FSM_H_INCLUDED
#define FSM_H_INCLUDED

#include <stdint.h>

typedef struct
{
	void (**func_vect)(void);
	uint8_t (*func_next)(uint8_t curr);
	volatile uint8_t curr;
} fsm_t;

typedef void (*state_func_t)(void);

void fsm_init(fsm_t *fsm, uint8_t (*func_next)(uint8_t curr), state_func_t func_vect[]);
void fsm_next(fsm_t *fsm);
void fsm_curr(fsm_t *fsm);
#endif //FSM_H_INCLUDED