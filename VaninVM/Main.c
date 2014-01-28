#include "OpCode.h"

#include "IOcode.h"
#include "CodeHeader.h"

#include "TOS.h"
#include "Context.h"
#include "ReturnStack.h"
#include "LocalVars.h"


long long cmp_int(long long* a, long long* b);
double cmp_double(double* a, double* b);

int main(int argc, char** argv)
{
	//ByteCode Variables
	FILE* input;

	//ExecutionProcess Variables
	double d1, d2;
	long long i1, i2;
	short s1, s2;
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


	if (argc<2)
	{
		printf("%s", "File is not specified");
		return 1;
	}

	fopen_s(&input, argv[1], "rb");
	//const_pull
	if (read_constant(input, &const_count, &const_index) != 0)
		return 1;
	//code_pull
	startfunc = read_bytecode(input, &phash_table);
	fclose(input);


	initTOS();
	initRStack();
	ip = 0;

	current_context = create_context(startfunc, phash_table, &code);
	node_last = NULL;

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
		case SLOAD0:
			//DO(SLOAD0, "Load empty string on TOS.", 1) 
			push_int((long long)(const_index[0]));
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
			d2 -= d1;
			push_double(d2);
			ip++; break;
		case ISUB:
			//DO(ISUB, "Subtract 2 ints on TOS (lower from upper), push value back.", 1)
			i1 = pop_int();
			i2 = pop_int();
			i2 -= i1;
			push_int(i2);
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
			d1 = d1/d2;
			push_double(d1);
			ip++; break;
		case IDIV:
			//DO(IDIV, "Divide 2 ints on TOS (upper to lower), push value back.", 1)
			i1 = pop_int();
			i2 = pop_int();
			i1 = i1/i2;
			push_int(i1);
			ip++; break;
		case IMOD:
			//DO(IMOD, "Modulo operation on 2 ints on TOS (upper to lower), push value back.", 1)
			i1 = pop_int();
			i2 = pop_int();
			i1 = i1 % i2;
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
			i1 = pop_int();
			i2 = pop_int();
			i1 = i1 & i2;
			push_int(i1);
			ip++; break;
		case IAXOR:
			//DO(IAXOR, "Arithmetic XOR of 2 ints on TOS, push value back.", 1)
			i1 = pop_int();
			i2 = pop_int();
			i1 = i1 ^ i2;
			push_int(i1);
			ip++; break;
		case IPRINT:
			//DO(IPRINT, "Pop and print integer TOS.", 1)
			i1 = pop_int();
			printf("%llu", i1);
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
		case S2I:
			//DO(S2I,  "Convert string on TOS to int.", 1)  
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
		case LOADDVAR0:
			//DO(LOADDVAR0, "Load double from variable 0, push on TOS.", 1)
			d1 = getlocal_double(current_context, 0);
			push_double(d1);
			ip++;break;
		case LOADDVAR1:
			//DO(LOADDVAR1, "Load double from variable 1, push on TOS.", 1)
			d1 = getlocal_double(current_context, 1);
			push_double(d1);
			ip++;break;
		case LOADDVAR2:
			//DO(LOADDVAR2, "Load double from variable 2, push on TOS.", 1)
			d1 = getlocal_double(current_context, 2);
			push_double(d1);
			ip++;break;
		case LOADDVAR3:
			//DO(LOADDVAR3, "Load double from variable 3, push on TOS.", 1)
			d1 = getlocal_double(current_context, 3);
			push_double(d1);
			ip++; break;
		case LOADIVAR0:
			//DO(LOADIVAR0, "Load int from variable 0, push on TOS.", 1)
			i1 = getlocal_int(current_context, 0);
			push_int(i1);
			ip++; break;
		case LOADIVAR1:
			//DO(LOADIVAR1, "Load int from variable 1, push on TOS.", 1)
			i1 = getlocal_int(current_context, 1);
			push_int(i1);
			ip++; break;
		case LOADIVAR2:
			//DO(LOADIVAR2, "Load int from variable 2, push on TOS.", 1) 
			i1 = getlocal_int(current_context, 2);
			push_int(i1);
			ip++; break;
		case LOADIVAR3:
			//DO(LOADIVAR3, "Load int from variable 3, push on TOS.", 1)
			i1 = getlocal_int(current_context, 3);
			push_int(i1);
			ip++; break;
		case LOADSVAR0:
			//DO(LOADSVAR0, "Load string from variable 0, push on TOS.", 1)
			i1 = (long long)getlocal_string(current_context, 0);
			push_int(i1);
			ip++; break;
		case LOADSVAR1:
			//DO(LOADSVAR1, "Load string from variable 1, push on TOS.", 1) 
			i1 = (long long)getlocal_string(current_context, 1);
			push_int(i1);
			ip++; break;
		case LOADSVAR2:
			//DO(LOADSVAR2, "Load string from variable 2, push on TOS.", 1)
			i1 = (long long)getlocal_string(current_context, 2);
			push_int(i1);
			ip++; break;
		case LOADSVAR3:
			//DO(LOADSVAR3, "Load string from variable 3, push on TOS.", 1)
			i1 = (long long)getlocal_string(current_context, 3);
			push_int(i1);
			ip++; break;
		case STOREDVAR0:
			//DO(STOREDVAR0, "Pop TOS and store to double variable 0.", 1) 
			d1 = pop_double();
			putlocal_double(&d1, current_context, 0);
			ip++; break;
		case STOREDVAR1:
			//DO(STOREDVAR1, "Pop TOS and store to double variable 1.", 1)
			d1 = pop_double();
			putlocal_double(&d1, current_context, 1);
			ip++; break;
		case STOREDVAR2:
			//DO(STOREDVAR2, "Pop TOS and store to double variable 2.", 1) 
			d1 = pop_double();
			putlocal_double(&d1, current_context, 2);
			ip++; break;
		case STOREDVAR3:
			//DO(STOREDVAR3, "Pop TOS and store to double variable 3.", 1)
			d1 = pop_double();
			putlocal_double(&d1, current_context, 3);
			ip++; break;
		case STOREIVAR0:
			//DO(STOREIVAR0, "Pop TOS and store to int variable 0.", 1)
			i1 = pop_int();
			putlocal_int(&i1, current_context, 0);
			ip++; break;
		case STOREIVAR1:
			//DO(STOREIVAR1, "Pop TOS and store to int variable 1.", 1)    
			i1 = pop_int();
			putlocal_int(&i1, current_context, 1);
			ip++; break;
		case STOREIVAR2:
			//DO(STOREIVAR2, "Pop TOS and store to int variable 2.", 1)
			i1 = pop_int();
			putlocal_int(&i1, current_context, 2);
			ip++; break;
		case STOREIVAR3:
			//DO(STOREIVAR3, "Pop TOS and store to int variable 3.", 1) 
			i1 = pop_int();
			putlocal_int(&i1, current_context, 3);
			ip++; break;
		case STORESVAR0:
			//DO(STORESVAR0, "Pop TOS and store to string variable 0.", 1)
			i1 = pop_int();
			putlocal_string((int*)&i1, current_context, 0);
			ip++; break;
		case STORESVAR1:
			//DO(STORESVAR1, "Pop TOS and store to string variable 1.", 1) 
			i1 = pop_int();
			putlocal_string((int*)&i1, current_context, 1);
			ip++; break;
		case STORESVAR2:
			//DO(STORESVAR2, "Pop TOS and store to string variable 2.", 1)  
			i1 = pop_int();
			putlocal_string((int*)&i1, current_context, 2);
			ip++; break;
		case STORESVAR3:
			//DO(STORESVAR3, "Pop TOS and store to string variable 3.", 1) 
			i1 = pop_int();
			putlocal_string((int*)&i1, current_context, 3);
			ip++; break;
		case LOADDVAR:
			//DO(LOADDVAR, "Load double from variable, whose 2-byte is id inlined to insn stream, push on TOS.", 3)
			s1 = *((short*)(code+(++ip)));
			d1 = getlocal_double(current_context, s1);
			push_double(d1);
			ip+=2; break;
		case LOADIVAR:
			//DO(LOADIVAR, "Load int from variable, whose 2-byte id is inlined to insn stream, push on TOS.", 3)
			s1 = *((short*)(code+(++ip)));
			i1 = getlocal_int(current_context, s1);
			push_int(i1);
			ip+=2; break;
		case LOADSVAR:
			//DO(LOADSVAR, "Load string from variable, whose 2-byte id is inlined to insn stream, push on TOS.", 3)
			s1 = *((short*)(code+(++ip)));
			i1 = (long long)getlocal_string(current_context, s1);
			push_int(i1);
			ip+=2; break;
		case STOREDVAR:
			//DO(STOREDVAR, "Pop TOS and store to double variable, whose 2-byte id is inlined to insn stream.", 3)
			s1 = *((short*)(code+(++ip)));
			d1 = pop_double();
			putlocal_double(&d1, current_context, s1);
			ip+=2; break;
		case STOREIVAR:
			//DO(STOREIVAR, "Pop TOS and store to int variable, whose 2-byte id is inlined to insn stream.", 3)
			s1 = *((short*)(code+(++ip)));
			i1 = pop_int();
			putlocal_int(&i1, current_context, s1);
			ip+=2; break;
		case STORESVAR:
			//DO(STORESVAR, "Pop TOS and store to string variable, whose 2-byte id is inlined to insn stream.", 3)
			s1 = *((short*)(code+(++ip)));
			i1 = pop_int();
			putlocal_string((int*)&i1, current_context, s1);
			ip+=2; break;
		case LOADCTXDVAR:
			//DO(LOADCTXDVAR, "Load double from variable, whose 2-byte context and 2-byte id inlined to insn stream, push on TOS.", 5)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			s2 = *((short*)(code+(ip)));
			if (find_context(s1) != NULL)
			{
				d1 = getlocal_double(find_context(s1), s2);
				push_double(d1);
			}
			else
			{
				printf("%s", "Context Not Found");
				exec_status = 0;
			}
			ip+=2; break;
		case LOADCTXIVAR:
			//DO(LOADCTXIVAR, "Load int from variable, whose 2-byte context and 2-byte id is inlined to insn stream, push on TOS.", 5)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			s2 = *((short*)(code+(ip)));
			if (find_context(s1) != NULL)
			{
				i1 = getlocal_int(find_context(s1), s2);
				push_int(i1);
			}
			else
			{
				printf("%s", "Context Not Found");
				exec_status = 0;
			}
			ip+=2; break;
		case LOADCTXSVAR:
			//DO(LOADCTXSVAR, "Load string from variable, whose 2-byte context and 2-byte id is inlined to insn stream, push on TOS.", 5)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			s2 = *((short*)(code+(ip)));
			if (find_context(s1) != NULL)
			{
				i1 = (long long)getlocal_string(find_context(s1), s2);
				push_double(i1);
			}
			else
			{
				printf("%s", "Context Not Found");
				exec_status = 0;
			}
			ip+=2; break;
		case STORECTXDVAR:
			//DO(STORECTXDVAR, "Pop TOS and store to double variable, whose 2-byte context and 2-byte id is inlined to insn stream.", 5)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			s2 = *((short*)(code+(ip)));
			d1 = pop_double();
			if (find_context(s1) != NULL)
			{
				putlocal_double(&d1, find_context(s1), s2);
			}
			else
			{
				printf("%s", "Context Not Found");
				exec_status = 0;
			}
			ip+=2; break;
		case STORECTXIVAR:
			//DO(STORECTXIVAR, "Pop TOS and store to int variable, whose 2-byte context and 2-byte id is inlined to insn stream.", 5) 
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			s2 = *((short*)(code+(ip)));
			i1 = pop_int();
			if (find_context(s1) != NULL)
			{
				putlocal_int(&i1, find_context(s1), s2);
			}
			else
			{
				printf("%s", "Context Not Found");
				exec_status = 0;
			}
			ip+=2; break;
		case STORECTXSVAR:
			//DO(STORECTXSVAR, "Pop TOS and store to string variable, whose 2-byte context and 2-byte id is inlined to insn stream.", 5)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			s2 = *((short*)(code+(ip)));
			i1 = pop_int();
			if (find_context(s1) != NULL)
			{
				putlocal_string((int*)&d1, find_context(s1), s2);
			}
			else
			{
				printf("%s", "Context Not Found");
				exec_status = 0;
			}
			ip+=2; break;
		case DCMP:
			//DO(DCMP, "Compare 2 topmost doubles, pushing libc-style comparator value cmp(upper, lower) as integer.", 1)
			d1 = TOS[tp-1];
			d2 = TOS[tp-2];
			push_double(cmp_double(&d1, &d2));
			ip++; break;
		case ICMP:
			//DO(ICMP, "Compare 2 topmost ints, pushing libc-style comparator value cmp(upper, lower) as integer.", 1)
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			push_int(cmp_int(&i1, &i2));
			ip++; break;
		case JA:
			//DO(JA, "Jump always, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip += (2+s1); break;
		case IFICMPNE:
			// DO(IFICMPNE, "Compare two topmost integers and jump if upper != lower, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			if (i1 != i2)
				ip += s1;
			break;
		case IFICMPE:
			//DO(IFICMPE, "Compare two topmost integers and jump if upper == lower, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			if (i1 == i2)
				ip += s1;
			break;
		case IFICMPG:
			//DO(IFICMPG, "Compare two topmost integers and jump if upper > lower, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			if (i1 > i2)
				ip += s1;
			break;
		case IFICMPGE:
			//DO(IFICMPGE, "Compare two topmost integers and jump if upper >= lower, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			if (i1 >= i2)
				ip += s1;
			break;
		case IFICMPL:
			//DO(IFICMPL, "Compare two topmost integers and jump if upper < lower, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			if (i1 < i2)
				ip += s1;
			break;
		case IFICMPLE:
			//DO(IFICMPLE, "Compare two topmost integers and jump if upper <= lower, next two bytes - signed offset of jump destination.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2;
			i1 = get_int(tp-1);
			i2 = get_int(tp-2);
			if (i1 <= i2)
				ip += s1;
			break;
		case DUMP:
			//DO(DUMP, "Dump value on TOS, without removing it.", 1) 
			i1 = get_int(tp-1);
			d1 = TOS[tp-1];
			printf("TOS: Double:%e Integer:%llu Hex:%#08llx", d1, i1, i1);
			ip++; break;
		case STOP:
			//DO(STOP, "Stop execution.", 1)
			exec_status = 0;				
			break;
		case CALL:
			//DO(CALL, "Call function, next two bytes - unsigned function id.", 3)
			s1 = *((short*)(code+(++ip)));
			ip+=2; push_ret(ip);
			push_context(current_context);
			current_context = create_context(s1, phash_table, &code);
			ip = 0; break;
		case RETURN:
			//DO(RETURN, "Return to call location", 1)
			remove_context(current_context);
			current_context = pop_context();
			code = phash_table[current_context->id]->code;
			ip = pop_ret();	break;
		case BREAK:
			//DO(BREAK, "Breakpoint for the debugger.", 1)
			getchar();
			ip++; break;
		}
	}
	remove_context(current_context);

	return 0;
}

double cmp_double(long long* a, long long* b)
{
	if (*a == *b) return 0;
	else if (*a > *b) return 1;
	else return -1;
}
long long cmp_int(long long* a, long long* b)
{
	if (*a == *b) return 0;
	else if (*a > *b) return 1;
	else return -1;
}