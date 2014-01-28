#ifndef CONTEXT_H
#define CONTEXT_H

#include "CodeHeader.h"
#include <stdio.h>

typedef union 
{
	double d_data;
	long long i_data;
	char* s_data;
} l_var;

typedef struct
{
	int id;
	l_var* locals;
} context;

typedef struct NODE
{
	context* obj;
	struct NODE* prev;
} c_node;

c_node* node_last;

context* create_context(int function, func** hash,char** code);
void push_context(context*);
context* pop_context();
void remove_context(context*);
context* find_context(int id);

#endif