#include "IOcode.h"

char* read_bytecode(char* path)
{
	FILE* input;
	char* buffer = (char*)calloc(1, 1000); //Early version without bytecode file
	fopen_s(&input, "bytecode", "rb");
	fread_s(buffer, 1000, sizeof(char), 1000, input); //Early version without bytecode file
	fclose(input);

	return buffer;
}
