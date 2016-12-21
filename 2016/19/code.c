#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int input = 3017957;

int main(int argc, char *argv[]) {
	int *Gifts;
	int i;
	int NumElves;
	int Next;
	int DEBUG=0;

	Gifts=(int*)malloc((input+1) * sizeof(*Gifts));
	for(i=0;i<input;i++) {
		Gifts[i]=1;
	}


	for(;;) {
		for(i=0;i<input;i++) {
			if(Gifts[i]) {
				for(int j=i+1; ; j++) {
					if(j == input) {
						j=0;
					}
					if(j == i) {
						printf("Unable to steal more for elf %i\n", i+1);
						goto Done;
					}
					if(Gifts[j]) {
						if(DEBUG) printf("Elf %i steals from Elf %i\n",
							 i+1, j+1);
						Gifts[i]+=Gifts[j];
						Gifts[j]=0;
						break;
					}
				}
			} else {
				if(DEBUG) printf("Elf %i skipped\n", i+1);
			}
		}
	}
Done:
	//DEBUG=1;
	puts("Day 2...");
	for(i=0;i<input;i++) {
		Gifts[i]=i+1;
	}
	NumElves=input;
	Next = 1;
	
	while(NumElves>1) {
		if(NumElves % 1000 == 0) {
			printf("%i Elves remaining... \n", NumElves);
		}
		for(i=0;i<NumElves;i++) {
			if(Gifts[i] >= Next) {
				Next = Gifts[i] + 1;
				break;
			}
		}
		if(i == NumElves) {
			i=0;
		}
		int Tgt = i+(NumElves/2);
		if(Tgt >= NumElves) {
			Tgt-=NumElves;
		}
		if(DEBUG) printf("Elf %i steals from Elf %i\n", Gifts[i], Gifts[Tgt]);
		memmove(&Gifts[Tgt], &Gifts[Tgt+1], (NumElves-Tgt-1)*sizeof(*Gifts));
		NumElves--;
	}
	printf("Remaining elf is %i\n", Gifts[0]);
	return 0;
}
