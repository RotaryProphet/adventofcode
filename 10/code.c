#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BufSize (1024 * 1024 * 10)
const char StartString[]="1113222113";

int Process(const char *Src, char *Dest) {
	int i, d=0;
	int num=1;
	char cur=Src[0];

	for(i=1;;i++) {
		if(Src[i] == cur) {
			num++;
		} else {
			d+=snprintf(&Dest[d], BufSize-d, "%i%c", num, cur);
			num=1;
			cur=Src[i];
		}

		if(d >= BufSize) {
			printf("Buffer too large!\n");
			exit(1);
		}

		if(!Src[i]) {
			break;
		}
	}
	return d;
}

int main(int argc, char *argv[]) {
	static char Buf1[BufSize];
	static char Buf2[BufSize];
	int i, l;

	strcpy(Buf1, StartString);

	for(i=0;i<50;i++) {
		l=Process(Buf1, Buf2);
		printf("%2i: %i\n", i, l);
		memset(Buf1, 0, sizeof(Buf1));
		strcpy(Buf1, Buf2);
		memset(Buf2, 0, sizeof(Buf2));
	}

	return 0;
}
