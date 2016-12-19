#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char input[]="10111100110001111";
const int TargetLength = 272;
const int Length2 = 35651584;

void process(int Length, char *Out) {
	char *Buf1, *Buf2;
	Buf1=malloc(Length*2+1);
	Buf2=malloc(Length*2+1);
	strcpy(Buf1, input);

	while(strlen(Buf1) < Length) {
		int l=strlen(Buf1);
		for(int i=0; i<l; i++) {
			Buf2[i] = Buf1[l-i-1] == '1' ? '0' : '1';
		}
		Buf2[l]=0;
		Buf1[l]='0';
		Buf1[l+1]=0;
		strcat(Buf1, Buf2);
		//printf("Length is %lu / %i\n", strlen(Buf1), Length);
	}

	Buf1[Length]=0;
	//printf("Length is %lu / %i\n", strlen(Buf1), Length);

	do {
		size_t l = strlen(Buf1);
		memset(Buf2, 0, Length*2+1);
		for(int i=0; i<l; i+=2) {
			Buf2[i/2] = (Buf1[i] == Buf1[i+1] ? '1' : '0');
		}
		strcpy(Buf1, Buf2);
		//printf("Length is %lu / %i\n", strlen(Buf1), Length);
	} while(strlen(Buf1)%2==0);
	strcpy(Out, Buf1);
}

int main(int argc, char *argv[]) {
	char Buffer[512];

	memset(Buffer, 0, sizeof(Buffer));
	process(TargetLength, Buffer);
	printf("Day 1 sum is '%s'\n", Buffer);

	process(Length2, Buffer);
	printf("Day 2 sum is '%s'\n", Buffer);
	
	
	return 0;
}
