#include <string.h>
#include "LocalVars.h"

long long getlocal_int(context* cont, int id)
{
	long long result;
	result = ((long long*)(cont->locals))[id];
	return result;
}

double getlocal_double(context* cont, int id)
{
	double result;
	result = ((double*)(cont->locals))[id];
	return result;
}

int getlocal_string(context* cont, int id)
{
	long long result;
	result = ((long long*)(cont->locals))[id];
	return (int)result;
}

void putlocal_int(long long* num, context* cont, int id)
{
	memcpy(((long long*)cont->locals)+id, num, sizeof(long long));
}

void putlocal_double(double* num, context* cont, int id)
{
	memcpy(((double*)cont->locals)+id, num, sizeof(double));
}

void putlocal_string(int* str, context* cont, int id)
{
	memcpy(((long long*)cont->locals)+id, (long long*)str, sizeof(long long));
}
