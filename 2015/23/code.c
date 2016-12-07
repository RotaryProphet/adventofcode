#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *strdup(const char *);

typedef enum _InstructionType {
	HALF,
	TRIPLE,
	INC,
	JUMP,
	JIE,
	JIO,
	IT_MAX
} IType;

typedef struct _Instruction {
	IType		Type;
	int		Reg;
	int		Offset;
	char		*Orig;
} Instruction;

Instruction *Program=NULL;
int NumInstructions=0;

unsigned int Registers[2];

//#define DBG(...) printf(__VA_ARGS__)
#define DBG(...)
void RunProgram() {
	int PC=0;

	while(PC >= 0 && PC < NumInstructions) {
		Instruction *This=&Program[PC];

		DBG("PC=%i '%s' [%u,%u]->", PC, This->Orig, Registers[0], Registers[1]); 

		switch(This->Type) {
		case HALF:
			Registers[This->Reg]/=2;
			PC++;
			break;
		case TRIPLE:
			Registers[This->Reg]*=3;
			PC++;
			break;
		case INC:
			Registers[This->Reg]++;
			PC++;
			break;
		case JUMP:
			PC+=This->Offset;
			break;
		case JIE:
			if(Registers[This->Reg]%2==0) {
				PC+=This->Offset;
			} else {
				PC++;
			}
			break;
		case JIO:
			if(Registers[This->Reg]==1) {
				PC+=This->Offset;
			} else {
				PC++;
			}
			break;
		default:
			printf("Unknown instruction '%i' at PC=%i\n", This->Type, PC);
			exit(1);
			break;
		}
		DBG("PC=%i [%u,%u]\n", PC, Registers[0], Registers[1]);
	}
	printf("Exiting loop, PC=%i\n", PC);
}

int main(int argc, char *argv[]) {
	char buffer[1024];

	while(fgets(buffer, sizeof(buffer), stdin)) {
		Instruction *This;
		char *c, *op1, *op2=NULL;
		Program = realloc(Program, (NumInstructions+1) * sizeof(*Program));
		This = &Program[NumInstructions++];

		memset(This, 0, sizeof(*This));

		c=strchr(buffer, '\n');
		*c=0;
		This->Orig = strdup(buffer);
		c=strchr(buffer, ' ');
		*c=0;
		op1=c+1;
		if((c=strchr(op1, ','))) {
			*c=0;
			op2=c+2;
		}

		if(!strcmp(buffer, "hlf")) {
			This->Type=HALF;
			This->Reg=*op1-'a';
		} else if(!strcmp(buffer, "tpl")) {
			This->Type=TRIPLE;
			This->Reg=*op1-'a';
		} else if(!strcmp(buffer, "inc")) {
			This->Type=INC;
			This->Reg=*op1-'a';
		} else if(!strcmp(buffer, "jmp")) {
			This->Type=JUMP;
			This->Offset=atoi(op1);
		} else if(!strcmp(buffer, "jie")) {
			This->Type=JIE;
			This->Reg=*op1-'a';
			This->Offset=atoi(op2);
		} else if(!strcmp(buffer, "jio")) {
			This->Type=JIO;
			This->Reg=*op1-'a';
			This->Offset=atoi(op2);
		} else {
			printf("Invalid instruction '%s'!\n", buffer);
			exit(1);
		}
	}
	
	memset(&Registers, 0, sizeof(Registers));	
	RunProgram();
	printf("Day 1: Value of 'b' is %u\n", Registers[1]);

	Registers[0]=1;
	Registers[1]=0;
	RunProgram();
	printf("Day 2: Value of 'b' is %u\n", Registers[1]);
	return 0;
}
