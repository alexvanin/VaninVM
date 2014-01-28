#ifndef LOCALVARS_H
#define LOCALVARS_H
#include "Context.h"

long long getlocal_int(context*, int);
double getlocal_double(context*, int);
int getlocal_string(context*, int);

void putlocal_int(long long*, context*, int);
void putlocal_double(double*, context*, int);
void putlocal_string(int*, context*, int);
#endif