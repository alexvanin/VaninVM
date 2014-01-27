#include <malloc.h>
#include "ReturnStack.h"

void push_ret(int num)
{
	RStack[rp++] = num;
}

int pop_ret()
{
	int num = RStack[--rp];
	return num;
}

int initRStack()
{
	RStack = (int*)calloc(1, 1000);
	rp = 0;
	return 0;
}