#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

int Registers[4];
typedef enum _InstType { CPY, JNZ, INC, DEC } InstType;
typedef struct _Inst {
	InstType	Type;
	int *A;
	int *B;
	int Consts[2];
} Inst;

Inst *Program=NULL;
size_t ProgramSize=0;

int main(int argc, char *argv[]) {
	char buf[1024]="";
	int inst=0;
	int PC=0;

	while(fgets(buf, sizeof(buf), stdin)) {
		char *Ptrs[32];
		int NPtrs=0;
		char *c;

		memset(Ptrs, 0, sizeof(Ptrs));

		if((c=strchr(buf, '\r'))) {
			*c=0;
		}
		if((c=strchr(buf, '\n'))) {
			*c=0;
		}
		
		for(c=buf; c && *c; ) {
			Ptrs[NPtrs++]=c;
			if((c=strchr(c, ' '))) {
				*c=0;
				c++;
			}
		}

		ProgramSize++;
		Program = (Inst*)realloc(Program, ProgramSize * sizeof(*Program));
		memset(&Program[inst], 0, sizeof(*Program));

		if(!strcmp(Ptrs[0], "cpy")) {
			Program[inst].Type = CPY;
		} else if(!strcmp(Ptrs[0], "jnz")) {
			Program[inst].Type = JNZ;
		} else if(!strcmp(Ptrs[0], "inc")) {
			Program[inst].Type = INC;
		} else if(!strcmp(Ptrs[0], "dec")) {
			Program[inst].Type = DEC;
		} else {
			printf("Unknown command '%s'\n", Ptrs[0]);
		}

		if(Ptrs[1][0] >= 'a' && Ptrs[1][0] <= 'd') {
			Program[inst].A = &Registers[ Ptrs[1][0]-'a' ];
		} else {
			Program[inst].A = &Program[inst].Consts[0];
			Program[inst].Consts[0] = atoi(Ptrs[1]);
		}

		if(Ptrs[2]) {
			if(Ptrs[2][0] >= 'a' && Ptrs[2][0] <= 'd') {
				Program[inst].B = &Registers[ Ptrs[2][0]-'a' ];
			} else {
				Program[inst].B = &Program[inst].Consts[1];
				Program[inst].Consts[1] = atoi(Ptrs[2]);
			}
		}
		inst++;
	}

	for(int i=0;i<2;i++) {
		memset(Registers, 0, sizeof(Registers));
		PC=0;
		if(i) {
			Registers[2]=1;
		}

		while(PC >= 0 && PC < ProgramSize) {
			Inst *P = &Program[PC];
			switch(P->Type) {
			case CPY:
				*(P->B) = *(P->A);
				PC++;
				break;
			case JNZ:
				if(*(P->A)) {
					PC+=*(P->B);
				} else {
					PC++;
				}
				break;
			case INC:
				(*(P->A))++;
				PC++;
				break;
			case DEC:
				(*(P->A))--;
				PC++;
				break;
			default:
				printf("Unknown command PC %i\n", PC);
				PC=-1;
			}
		}

		printf("Day %i: Value of A register is %i\n", i+1, Registers[0]);
	}

	return 0;
}












