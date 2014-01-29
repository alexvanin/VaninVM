#include <malloc.h>
#include "TOS.h"

void push_int(long long num)
{
	tos_num number;
	number.num_i = num;
	TOS[tp++] = number.num_d;
}

long long pop_int()
{
	tos_num number;
	number.num_d =  TOS[--tp];
	return number.num_i;
}

long long get_int(int id)
{
	tos_num number;
	number.num_d =  TOS[id];
	return number.num_i;
}

void push_double(double num)
{
	TOS[tp++] = num;
}

double pop_double()
{
	double num =  TOS[--tp];
	return num;
}

int initTOS(int count)
{
	TOS = (double*)calloc(count, sizeof(double*));
	tp = 0;
	return 0;
}