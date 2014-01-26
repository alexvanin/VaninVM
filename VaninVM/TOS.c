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

void push_double(double num)
{
	TOS[tp++] = num;
}

double pop_double()
{
	double num =  TOS[--tp];
	return num;
}

int initTOS()
{
	TOS = (double*)calloc(1, 12000);
	tp = 0;
	return 0;
}