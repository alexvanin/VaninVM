#include "IOcode.h"
#include "CodeHeader.h"
#include <string.h>

int read_bytecode(FILE* stream, func*** hash)
{
	int i;
	char* name;
	char* code;
	func* function;

	*(hash) = (func**)malloc(sizeof(func*)*65536);
	fread_s(&ByteCodeH_Common, sizeof(ByteCodeH_Common), sizeof(ByteCodeH_Common), 1, stream);

	for (i=0; i<ByteCodeH_Common.count_code; i++)
	{
		function = (func*)malloc(sizeof(func));

		fread(&ByteCodeH_Signat, sizeof(ByteCodeH_Signat), 1, stream);
		name = (char*)malloc(ByteCodeH_Signat.size_signature);
		fread(name, ByteCodeH_Signat.size_signature, 1, stream);
		free(name);

		fread(&ByteCodeH_Primary, sizeof(ByteCodeH_Primary), 1, stream);
		code = (char*)malloc(ByteCodeH_Signat.size_bytecode);
		fread(code, ByteCodeH_Signat.size_bytecode, 1, stream);

		function->code=code;
		function->count_args=ByteCodeH_Primary.count_args;
		function->count_locals=ByteCodeH_Primary.count_locals;
		(*(hash))[ByteCodeH_Primary.id]=function;
	}
	return ByteCodeH_Common.id_start;

}

int read_constant(FILE* stream, int* count, char*** index)
{
	int i, j;
	char* buffer;

	fread_s(&First_Header, sizeof(First_Header), sizeof(First_Header), 1, stream); //Reading first part of header
	*(count) = First_Header.count_const;

	buffer = (char*)malloc(First_Header.size_const+1);
	buffer[0]=0;
	*(index) = (char**)malloc(sizeof(char**)*First_Header.count_const);


	fread_s(buffer+1, First_Header.size_const, sizeof(char), First_Header.size_const, stream); //Reading constant values

	j=0;
	(*(index))[j++]=buffer;
	for (i = 0; i<First_Header.size_const+1; i++)
	{
		if (j==First_Header.count_const)
			break;

		if (buffer[i] == 0)
			(*(index))[j++]=&buffer[i+1];
	}
	return 0;
}
