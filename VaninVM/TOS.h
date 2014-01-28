#ifndef TOS_H
#define TOS_H

typedef union 
{
	double num_d;
	long long num_i;
} tos_num;

double* TOS;
int tp;
int initTOS();
void push_int(long long);
void push_double(double);
long long pop_int();
long long get_int(int);
double pop_double();

#endif