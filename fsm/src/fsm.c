#include "fsm.h"

void fsm_init(fsm_t *fsm, uint8_t (*func_next)(uint8_t curr), state_func_t func_vect[])
{
	fsm->curr = 0;
	fsm->func_next = func_next;
	fsm->func_vect = func_vect;
}

void fsm_next(fsm_t *fsm)
{
	fsm->curr = fsm->func_next(fsm->curr);
}

void fsm_curr(fsm_t *fsm)
{
	fsm->func_vect[fsm->curr]();
}