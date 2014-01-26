#ifndef TOS_H
#define TOS_H

typedef union 
{
	double num_d;
	int num_i[2];
} tos_num;

double* TOS;
int tp;
int initTOS();
void push_int(int);
void push_double(double);
int pop_int();
double pop_double();

#endif