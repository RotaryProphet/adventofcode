#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int code[3][3]={
	{1,2,3},
	{4,5,6},
	{7,8,9}
};

char code2[7][8]={
	"0000000",
	"0001000",
	"0023400",
	"0567890",
	"00ABC00",
	"000D000",
	"0000000"
};

int main(int argc, char *argv[]) {
	int x=1, y=1;
	int x2=3, y2=1;
	int c;
	char Code[10]="";
	char Code2[10]="";
	int p=0;

	while((c=getchar())>0) {
		switch(c) {
		case '\n':
			Code[p]='0' + code[x][y];
			Code2[p++]=code2[x2][y2];
			break;
		case 'U':
			if(--x < 0) {
				x=0;
			}
			if(code2[x2-1][y2] > '0') {
				x2--;
			}
			break;
		case 'D':
			if(++x > 2) {
				x=2;
			}
			if(code2[x2+1][y2] > '0') {
				x2++;
			}
			break;
		case 'L':
			if(--y < 0) {
				y=0;
			}
			if(code2[x2][y2-1] > '0') {
				y2--;
			}
			break;
		case 'R':
			if(++y > 2) {
				y=2;
			}
			if(code2[x2][y2+1] > '0') {
				y2++;
			}
			break;
		default:
			printf("unknown character '%c' %i\n", c, c);
		}
	}
	printf("Code 1 '%s'\nCode 2 '%s'\n", Code, Code2);
	return 0;
}
			
