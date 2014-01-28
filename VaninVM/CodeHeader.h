#ifndef CODE_HEADER
#define CODE_HEADER
#pragma pack(push, 1)
struct 
{
	char signature[2];
	int version;
	int count_const;
	int size_const;
}Const_Header;

struct
{
	short id_start;
	int count_code;
}ByteCodeH_Common;

struct
{
	int size_func;
	int size_bytecode;
	int size_signature;
}ByteCodeH_Signat;

struct
{
	short id;
	int count_locals;
	int count_args;
}ByteCodeH_Primary;

typedef struct
{
	int count_locals;
	int count_args;
	char* code;
} func;
#endif
#pragma pop