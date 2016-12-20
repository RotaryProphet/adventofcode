#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROWS 40

const char input[]="^^^^......^...^..^....^^^.^^^.^.^^^^^^..^...^^...^^^.^^....^..^^^.^.^^...^.^...^^.^^^.^^^^.^^.^..^.^";
int COLS;

int process(char *Prev, char *Next) {
	int r=0;
	Next[0]='|';
	Next[COLS+1]='|';
	Next[COLS+2]=0;
	for(int i=1; i<=COLS; i++) {
		if(	(Prev[i-1] == '^' && Prev[i] == '^' && Prev[i+1] != '^')
		   ||	(Prev[i-1] != '^' && Prev[i] == '^' && Prev[i+1] == '^')
		   ||	(Prev[i-1] == '^' && Prev[i] != '^' && Prev[i+1] != '^')
		   ||	(Prev[i-1] != '^' && Prev[i] != '^' && Prev[i+1] == '^')
		) {
			Next[i]='^';
		} else {
			Next[i]='.';
			r++;
		}
	}
	return r;
}

int main(int argc, char *argv[]) {
	int safe=0;
	char Prev[512], Next[512];
	COLS=strlen(input);
	
	strcpy(Prev, "|");
	strcat(Prev, input);
	strcat(Prev, "|");

	//puts(Prev);
	for(int i=1;i<=COLS;i++) {
		if(Prev[i] == '.') {
			safe++;
		}
	}
	
	for(int r=1;r<ROWS;r++) {
		safe+=process(Prev, Next);
		strcpy(Prev, Next);
		//puts(Prev);
	}
	printf("Day 1 %i safe tiles\n", safe);

	for(int r=ROWS;r<400000;r++) {
		safe+=process(Prev, Next);
		strcpy(Prev, Next);
	}
	printf("Day 2 %i safe tiles\n", safe);
	
	return 0;
}
