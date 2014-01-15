#include "IOcode.h"
#include "TOS.h"
#include "OpCode.h"


int main(int argc, char** argv)
{
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

			case STOP:
				exec_status = 0;				
				break;			
		}
	}
	printf("%f", pop_double());
	getchar();
	return 0;
	
}