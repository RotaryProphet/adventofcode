#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define DEBUG 0

int Registers[4];
char Names[][4]={"CPY","JNZ","INC","DEC","TGL"};
typedef enum _InstType { CPY, JNZ, INC, DEC, TGL } InstType;
typedef struct _Inst {
	InstType	Type;
	int *A;
	int *B;
	int Consts[2];
	char AName[16];
	char BName[16];
} Inst;

Inst *Program=NULL;
size_t ProgramSize=0;

int main(int argc, char *argv[]) {
	char buf[1024]="";
	int inst=0;
	int PC=0;
	Inst *ProgramBak=NULL;

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
		if(NPtrs < 2) {
			continue;
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
		} else if(!strcmp(Ptrs[0], "tgl")) {
			Program[inst].Type = TGL;
		} else {
			printf("Unknown command '%s'\n", Ptrs[0]);
			exit(1);
		}

		if(Ptrs[1][0] >= 'a' && Ptrs[1][0] <= 'd') {
			Program[inst].A = &Registers[ Ptrs[1][0]-'a' ];
		} else {
			Program[inst].A = &Program[inst].Consts[0];
			Program[inst].Consts[0] = atoi(Ptrs[1]);
		}
		strcpy(Program[inst].AName, Ptrs[1]);

		if(Ptrs[2]) {
			if(Ptrs[2][0] >= 'a' && Ptrs[2][0] <= 'd') {
				Program[inst].B = &Registers[ Ptrs[2][0]-'a' ];
			} else {
				Program[inst].B = &Program[inst].Consts[1];
				Program[inst].Consts[1] = atoi(Ptrs[2]);
			}
			strcpy(Program[inst].BName, Ptrs[2]);
		} else {
			Program[inst].BName[0]=0;
		}
		inst++;
	}

	if((ProgramBak=(Inst*)malloc(ProgramSize * sizeof(*Program)))==NULL) {
		fprintf(stderr, "Unable to allocate\n");
		exit(1);
	}
	memcpy(ProgramBak, Program, ProgramSize*sizeof(*Program));

	for(int i=0;i<2;i++) {
		memcpy(Program, ProgramBak, ProgramSize*sizeof(*Program));
		int Count=0;
		memset(Registers, 0, sizeof(Registers));
		if(i==0) {
			Registers[0]=7;
		} else {
			Registers[0]=12;
		}
		PC=0;

		while(PC >= 0 && PC < ProgramSize) {
			int tgt;
			Inst *P = &Program[PC];

			if(DEBUG) {
				int r;
				printf("%05i PC=0x%02x %s %s(%i)\t",
					Count++, PC, 
					Names[P->Type], P->AName, *(P->A));

				if(P->B) {
					printf(" %s(%i)\t", P->BName, *(P->B));
				} else {
					printf("\t");
				}
				for(r=0;r<4;r++) {
					printf("%c=0x%04x ", r+'a', Registers[r]);
				}
				puts("");
			}


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
				if(*(P->A) == INT_MAX) {
					fprintf(stderr,"Overflow!\n");
					exit(1);
				}
				(*(P->A))++;
				PC++;
				break;
			case DEC:
				if(*(P->A) == INT_MIN) {
					fprintf(stderr, "Underflow!\n");
					exit(1);
				}
				(*(P->A))--;
				PC++;
				break;
			case TGL:
				tgt=PC + *(P->A);
				if(tgt >= 0 && tgt < ProgramSize) {
					Inst *T = &Program[tgt];
					switch(T->Type) {
					case JNZ:
						T->Type = CPY;
						break;
					case CPY:
						T->Type = JNZ;
						break;
					case INC:
						T->Type = DEC;
						break;
					case DEC:
					case TGL:
						T->Type = INC;
						break;
					default:
						fprintf(stderr, "Unknown command toggling instruction %i\n", tgt);
						exit(1);
					}
				}
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
