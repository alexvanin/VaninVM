#include <string.h>
#include "LocalVars.h"

long long getlocal_int(context* cont, int id)
{
	long long result;
	result = (cont->locals)[id].i_data;
	return result;
}

double getlocal_double(context* cont, int id)
{
	double result;
	result = (cont->locals)[id].d_data;
	return result;
}

char* getlocal_string(context* cont, int id)
{
	char* result;
	result = (cont->locals)[id].s_data;
	return result;
}

void putlocal_int(long long* num, context* cont, int id)
{
	//memcpy(((long long*)cont->locals)+id, num, sizeof(long long));
	memcpy((cont->locals)+id, num, sizeof (long long));
}

void putlocal_double(double* num, context* cont, int id)
{
	memcpy((cont->locals)+id, num, sizeof(double));
}

void putlocal_string(char* str, context* cont, int id)
{
	memcpy((cont->locals)+id, str, sizeof(int*));
}
