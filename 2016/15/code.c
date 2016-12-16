#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _Disc {
	int NumPositions;
	int TZPosition;
	int ReqPos;
} Disc;

Disc *Discs=NULL;
int NumDiscs=0;

int main(int argc, char *argv[]) {
	char buf[1024]="";
	int t;
	int Day=0;

	Discs=(Disc*)malloc(sizeof(*Discs));
	memset(Discs, 0, sizeof(*Discs));
	NumDiscs++;

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
		
		Discs = (Disc*)realloc(Discs, (NumDiscs+1)*sizeof(*Discs));
		Discs[NumDiscs].NumPositions=atoi(Ptrs[3]);
		Discs[NumDiscs].TZPosition=atoi(Ptrs[11]);
		Discs[NumDiscs].ReqPos = Discs[NumDiscs].NumPositions - NumDiscs;
		while(Discs[NumDiscs].ReqPos < 0) {
			Discs[NumDiscs].ReqPos += Discs[NumDiscs].NumPositions;
		}

		printf("Disc %i\nNumPositions=%i\nTZPosition=%i\nReqPos=%i\n", NumDiscs, 
			Discs[NumDiscs].NumPositions, Discs[NumDiscs].TZPosition,
			Discs[NumDiscs].ReqPos);
		NumDiscs++;
	}

	puts("");

	do {
		for(t=0;;t++) {
			int d;
			for(d=1;d<NumDiscs;d++) {
				int pos = (Discs[d].TZPosition + t) % Discs[d].NumPositions;
				if(pos != Discs[d].ReqPos) {
					break;
				}
			}
			if(d == NumDiscs) {
				printf("Day %i time is %i\n", ++Day, t);
				break;
			}
		}

		Discs = (Disc*)realloc(Discs, (NumDiscs+1)*sizeof(*Discs));
		Discs[NumDiscs].NumPositions=11;
		Discs[NumDiscs].TZPosition=0;
		Discs[NumDiscs].ReqPos = Discs[NumDiscs].NumPositions - NumDiscs;
		NumDiscs++;
	} while(Day < 2);
	return 0;
}












