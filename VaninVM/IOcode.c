#include "IOcode.h"
#include "CodeHeader.h"

char* read_bytecode(FILE* stream)
{
	char* buffer = (char*)calloc(1, 1000); //Early version without bytecode file
	fread_s(buffer, 1000, sizeof(char), 1000, stream); //Early version without bytecode file

	return buffer;
}

int read_constant(FILE* stream, int* count, char*** index)
{
	int i, j;
	char* buffer;

	fread_s(&First_Header, sizeof(First_Header), sizeof(First_Header), 1, stream); //Reading first part of header
	*(count) = First_Header.count_const;

	buffer = (char*)malloc(First_Header.size_const);
	*(index) = (char**)malloc(sizeof(char**)*2);


	fread_s(buffer, First_Header.size_const, sizeof(char), First_Header.size_const, stream); //Reading constant values

	j=0;
	if (First_Header.count_const>0)
	{
		(*(index))[j++] = buffer;
		for (i = 0; i<First_Header.size_const; i++)
		{
			if (j==First_Header.count_const)
				break;

			if (buffer[i] == 0)
				(*(index))[j++]=&buffer[i+1];
		}
	}
	return 0;
}
