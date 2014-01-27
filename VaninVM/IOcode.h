#ifndef IOCODE_H
#define IOCODE_H

#include <malloc.h>
#include <stdio.h>

char* read_bytecode(FILE*);
int read_constant(FILE*, int*, char***);
#endif