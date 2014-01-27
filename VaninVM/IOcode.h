#ifndef IOCODE_H
#define IOCODE_H

#include "CodeHeader.h"
#include <malloc.h>
#include <stdio.h>

int read_bytecode(FILE*, func***);
int read_constant(FILE*, int*, char***);
#endif