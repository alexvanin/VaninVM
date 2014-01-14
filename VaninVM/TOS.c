#include <malloc.h>
#include "TOS.h"

void push_int(int num)
{
	TOS[tp++] = num;
}

int pop_int()
{
	int num =  TOS[--tp];
	return num;
}

void push_double(double num)
{
	tos_num cont = { num };
	push_int(cont.num_i[0]);
	push_int(cont.num_i[1]);
}

double pop_double()
{
	tos_num cont;
	cont.num_i[1] = pop_int();
	cont.num_i[0] = pop_int();
	return cont.num_d;
}

int initTOS()
{
	TOS = (int*)calloc(1, 1500);
	tp = 0;
	return 0;
}