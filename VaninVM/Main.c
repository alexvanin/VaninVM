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
				d1 = pop_double();
				d2 = pop_double();
				d1 += d2;
				push_double(d1);
				ip++; break;
			case IADD:
				i1 = pop_int();
				i2 = pop_int();
				i1 += i2;
				push_int(i1);
				ip++; break;
			case DSUB:
				d1 = pop_double();
				d2 = pop_double();
				d1 -= d2;
				push_double(d1);
				ip++; break;
			case ISUB:
				i1 = pop_int();
				i2 = pop_int();
				i1 -= i2;
				push_int(i1);
				ip++; break;
			case DMUL:
				d1 = pop_double();
				d2 = pop_double();
				d1 *= d2;
				push_double(d1);
				ip++; break;
			case IMUL:
				i1 = pop_int();
				i2 = pop_int();
				i1 *= i2;
				push_int(i1);
				ip++; break;
			case DDIV:
				d1 = pop_double();
				d2 = pop_double();
				d1 /= d2;
				push_double(d1);
				ip++; break;
			case IDIV:
				i1 = pop_int();
				i2 = pop_int();
				i1 /= i2;
				push_int(i1);
				ip++; break;
			case IMOD:
				i1 = pop_int();
				i2 = pop_int();
				i1 %= i2;
				push_int(i1);
				ip++; break;
			case DNEG:
				d1 = pop_double();
				d1 = -d1;
				push_double(d1);
				ip++; break;
			case INEG:
				i1 = pop_int();
				i1 = - i1;
				push_int(i1);
				ip++; break;
			case IAOR:
				i1 = pop_int();
				i2 = pop_int();
				i1 = i1 | i2;
				push_int(i1);
				ip++;break;
			case DPRINT:
				printf("%f", pop_double());
				ip++; break;
			case IPRINT:
				printf("%d", pop_int());
				ip++; break;
			case STOP:
				exec_status = 0;				
				break;			
		}
	}
	getchar();
	return 0;
	
}