#include "IOcode.h"
#include "TOS.h"
#include "OpCode.h"


int main(int argc, char** argv)
{
	double d1, d2;
	int i1, i2;
	char* code;	
	int ip;


	int exec_status = 1;

	code = read_bytecode("bytecode");
	initTOS();

	ip = 0;

	while (exec_status)
	{
		switch (code[ip])
		{
			case DLOAD0:
				// DO(DLOAD0, "Load double 0 on TOS.", 1)
				push_double(0);
				ip++; break;
			case ILOAD0:
				 //DO(ILOAD0, "Load int 0 on TOS.", 1)
				push_int(0);
				ip++; break;
			case DLOAD1:
				//DO(DLOAD1, "Load double 1 on TOS.", 1)  
				push_double(1);
				ip++; break;
			case ILOAD1:
				//DO(ILOAD1, "Load int 1 on TOS.", 1)
				push_int(1);
				ip++; break;
			case DLOADM1:
				//DO(DLOADM1, "Load double -1 on TOS.", 1) 
				push_double(-1);
				ip++; break;
			case ILOADM1:
				//DO(ILOADM1, "Load int -1 on TOS.", 1)
				push_int(-1);
				ip++; break;
			case DADD:
				//DO(DADD, "Add 2 doubles on TOS, push value back.", 1)
				d1 = pop_double();
				d2 = pop_double();
				d1 += d2;
				push_double(d1);
				ip++; break;
			case IADD:
				//DO(IADD, "Add 2 ints on TOS, push value back.", 1)
				i1 = pop_int();
				i2 = pop_int();
				i1 += i2;
				push_int(i1);
				ip++; break;
			case DSUB:
				//DO(DSUB, "Subtract 2 doubles on TOS (lower from upper), push value back.", 1)
				d1 = pop_double();
				d2 = pop_double();
				d1 -= d2;
				push_double(d1);
				ip++; break;
			case ISUB:
				//DO(ISUB, "Subtract 2 ints on TOS (lower from upper), push value back.", 1)
				i1 = pop_int();
				i2 = pop_int();
				i1 -= i2;
				push_int(i1);
				ip++; break;
			case DMUL:
				//DO(DMUL, "Multiply 2 doubles on TOS, push value back.", 1)
				d1 = pop_double();
				d2 = pop_double();
				d1 *= d2;
				push_double(d1);
				ip++; break;
			case IMUL:
				//DO(IMUL, "Multiply 2 ints on TOS, push value back.", 1)
				i1 = pop_int();
				i2 = pop_int();
				i1 *= i2;
				push_int(i1);
				ip++; break;
			case DDIV:
				//DO(DDIV, "Divide 2 doubles on TOS (upper to lower), push value back.", 1)
				d1 = pop_double();
				d2 = pop_double();
				d1 /= d2;
				push_double(d1);
				ip++; break;
			case IDIV:
				//DO(IDIV, "Divide 2 ints on TOS (upper to lower), push value back.", 1)
				i1 = pop_int();
				i2 = pop_int();
				i1 /= i2;
				push_int(i1);
				ip++; break;
			case IMOD:
				//DO(IMOD, "Modulo operation on 2 ints on TOS (upper to lower), push value back.", 1)
				i1 = pop_int();
				i2 = pop_int();
				i1 %= i2;
				push_int(i1);
				ip++; break;
			case DNEG:
				//DO(DNEG, "Negate double on TOS.", 1)
				d1 = pop_double();
				d1 = -d1;
				push_double(d1);
				ip++; break;
			case INEG:
				//DO(INEG, "Negate int on TOS.", 1)
				i1 = pop_int();
				i1 = - i1;
				push_int(i1);
				ip++; break;
			case IAOR:
				//DO(IAOR, "Arithmetic OR of 2 ints on TOS, push value back.", 1)
				i1 = pop_int();
				i2 = pop_int();
				i1 = i1 | i2;
				push_int(i1);
				ip++; break;
			case IAAND:
				//DO(IAAND, "Arithmetic AND of 2 ints on TOS, push value back.", 1)
				ip++; break;
			case IAXOR:
				//DO(IAXOR, "Arithmetic XOR of 2 ints on TOS, push value back.", 1)
			case IPRINT:
				//DO(IPRINT, "Pop and print integer TOS.", 1)
				i1 = pop_int();
				push_int(i1);
				printf("%d", i1);
				ip++; break;
			case DPRINT:
				//DO(DPRINT, "Pop and print double TOS.", 1)
				d1 = pop_double();
				push_double(d1);
				printf("%f", d1);
				ip++; break;
			case STOP:
				//DO(STOP, "Stop execution.", 1)
				exec_status = 0;				
				break;			
		}
	}
	return 0;
}