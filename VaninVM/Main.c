#include "IOcode.h"
#include "TOS.h"


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
			case 1:
				push_double(12.1);
				ip++;
				break;

			case 78:
				exec_status = 0;				
				break;			
		}
	}
	printf("%f", pop_double());
	getchar();
	return 0;
	
}