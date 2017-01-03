#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#define MAXLEN 16
#define LASTCHAR 'h'
char start[]="abcdefgh";
char target[]="fbgdceah";

void swappos(char *pass, int a, int b) {
	char c=pass[a];
	pass[a]=pass[b];
	pass[b]=c;
}

void swapchar(char *pass, int a, int b) {
	char *p1, *p2;
	p1=strchr(pass, a);
	p2=strchr(pass, b);
	*p1=b;
	*p2=a;
}

void rev(char *pass, int a, int b) {
	while(a < b) {
		swappos(pass, a++, b--);
	}
}

void rotate(char *pass, bool right, int a) {
	size_t l=strlen(pass);
	while(a--) {
		if(!right) {
			pass[l]=pass[0];
			memmove(&pass[0], &pass[1], l);
			pass[l]=0;
		} else {
			memmove(&pass[1], &pass[0], l);
			pass[0]=pass[l];
			pass[l]=0;
		}
	}
}

void rotatepos(char *pass, int a) {
	int ix=strchr(pass, a)-pass;
	if(ix >= 4) {
		ix+=2;
	} else {
		ix++;
	}
	rotate(pass, 1, ix);
}

void move(char *pass, int a, int b) {
	size_t l=strlen(pass);
	char c=pass[a];
	memmove(&pass[a], &pass[a+1], l-a-1);
	memmove(&pass[b+1], &pass[b], l-b-1);
	pass[b]=c;
	pass[l]=0;
}

char *scramble(char *start, char *filename) {
	char buf[1024]="";
	static char pass[MAXLEN];
	FILE *f;
	strcpy(pass, start);

	if((f=fopen(filename,"r"))==NULL) {
		fprintf(stderr, "Unable to open '%s' for reading\n", filename);
		exit(1);
	}
	while(fgets(buf, sizeof(buf), f)) {
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
		if(!strcmp(Ptrs[0], "swap")) {
			if(!strcmp(Ptrs[1], "position")) {
				swappos(pass, atoi(Ptrs[2]), atoi(Ptrs[5]));
			} else {
				swapchar(pass, *Ptrs[2], *Ptrs[5]);
			}
		} else if(!strcmp(Ptrs[0], "rotate")) {
			if(!strcmp(Ptrs[1], "left")) {
				rotate(pass, 0, atoi(Ptrs[2]));
			} else if(!strcmp(Ptrs[1], "right")) {
				rotate(pass, 1, atoi(Ptrs[2]));
			} else {
				rotatepos(pass, *Ptrs[6]);
			}
		} else if(!strcmp(Ptrs[0], "reverse")) {
			rev(pass, atoi(Ptrs[2]), atoi(Ptrs[4]));
		} else if(!strcmp(Ptrs[0], "move")) {
			move(pass, atoi(Ptrs[2]), atoi(Ptrs[5]));
		}
	}
	fclose(f);
	return pass;
}

int iterate(char *pass, size_t l) {
	for(size_t i=0;i<l;i++) {
		pass[i]++;
		if(pass[i] > LASTCHAR) {
			if(i==l-1) {
				return 1;
			}
			pass[i]='a';
			continue;
		}
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	char pass[MAXLEN];
	size_t l=strlen(start);
	int c;
	printf("Day 1 Pass is '%s'\n", scramble(start, argv[1]));

	memset(pass, 'a', l);
	pass[l]=0;
	do {
		for(c='a'; c<=LASTCHAR; c++) {
			if(!strchr(pass, c)) {
				break;
			}
		}
		if(c>LASTCHAR) {
			puts(pass);
			if(!strcmp(target, scramble(pass,argv[1]))) {
				printf("Day 2: Got %s with %s\n", target, pass);
				break;
			}
		}
	} while(!iterate(pass, l));
	
	return 0;
}












