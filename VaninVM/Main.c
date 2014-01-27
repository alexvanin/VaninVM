#include "IOcode.h"
#include "TOS.h"
#include "OpCode.h"
#include "CodeHeader.h"
#include "Context.h"
#include "ReturnStack.h"



int main(int argc, char** argv)
{
	//ByteCode Variables
	FILE* input;

	//ExecutionProcess Variables
	double d1, d2;
	long long i1, i2;
	short s1;
	char* code;	
	int ip, startfunc;

	//ConstSection Variables
	int const_count;
	char** const_index;

	//CodeSection Variables
	func** phash_table;
	context* current_context;

	//Running Variable
	int exec_status = 1;

	fopen_s(&input, "bytecode", "rb");
	//const_pull
	read_constant(input, &const_count, &const_index);
	//code_pull
	startfunc = read_bytecode(input, &phash_table);
	fclose(input);
	

	initTOS();
	initRStack();
	ip = 0;

	current_context = create_context(startfunc, phash_table, &code);

	while (exec_status)
	{
		switch (code[ip])
		{
			case INVALID:
				//DO(INVALID, "Invalid instruction.", 1)
				ip++; break;
			case DLOAD:
				//DO(DLOAD, "Load double on TOS, inlined into insn stream.", 9)
				d1 = *((double*)(code+(++ip)));
				push_double(d1);
				ip+=8; break;
			case ILOAD:
				//DO(ILOAD, "Load int on TOS, inlined into insn stream.", 9)
				i1 = *((long long*)(code+(++ip)));
				push_int(i1);
				ip+=8; break;
			case SLOAD:
				s1 = *((short*)(code+(++ip)));
				push_int((long long)(const_index[s1]));
				ip+=2; break;
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
				printf("%d", i1);
				ip++; break;
			case DPRINT:
				//DO(DPRINT, "Pop and print double TOS.", 1)
				d1 = pop_double();
				printf("%f", d1);
				ip++; break;
			case SPRINT:
				//DO(SPRINT, "Pop and print string TOS.", 1)
				i1 = pop_int();
				printf("%s", (char*)i1);
				ip++; break;
			case I2D:
				//DO(I2D,  "Convert int on TOS to double.", 1)
				i1 = pop_int();
				d1 = (double)i1;
				push_double(d1);
				ip++; break;
			case D2I:
				//DO(D2I,  "Convert double on TOS to int.", 1)
				d1 = pop_double();
				i1 = (int)d1;
				push_int(i1);
				ip++; break;
			case SWAP:
				//DO(SWAP, "Swap 2 topmost values.", 1)
				i1 = pop_int();
				i2 = pop_int();
				push_int(i1);
				push_int(i2);
				ip++; break;
			case POP:
				//DO(POP, "Remove topmost value.", 1)
				TOS--;
				ip++; break;
			case STOP:
				//DO(STOP, "Stop execution.", 1)
				exec_status = 0;				
				break;
			case CALL:
				s1 = *((short*)(code+(++ip)));
				ip+=2; push_ret(ip);
				push_context(current_context);
				current_context = create_context(s1, phash_table, &code);
				ip = 0; break;
			case RETURN:
				remove_context(current_context);
				current_context = pop_context();
				code = phash_table[current_context->id]->code;
				ip = pop_ret();	break;

		}
	}
	remove_context(current_context);
	getchar();
	return 0;
}