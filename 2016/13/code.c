#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a,b) (a > b ? b : a)
#define MAX(a,b) (a > b : a : b)

const unsigned int TargetX = 31;
const unsigned int TargetY = 39;
const unsigned int MagicNumber = 1352;

typedef struct _State {
	unsigned int X;
	unsigned int Y;
	unsigned int Steps;
} State;

State *History=NULL;
size_t StepSize=0;
size_t NumSteps=0;
char Maze[1000][1000];

unsigned int Day2=0;
int Found=0;

char IsWall(unsigned int x, unsigned int y);

void AddToState(unsigned int x, unsigned int y, unsigned int Steps) {
	if(Maze[x][y] == '#') {
		return;
	} else if(!Maze[x][y]) {
		if((Maze[x][y]=IsWall(x,y))=='#') {
			return;
		}
	} else {
		for(int i=0;i<NumSteps;i++) {
			if(History[i].X == x && History[i].Y == y && History[i].Steps <= Steps) {
				return;
			}
		}
	}

	if(StepSize == NumSteps) {
		StepSize+=128;
		History=realloc(History, StepSize*sizeof(*History));
	}
	History[NumSteps].X=x;
	History[NumSteps].Y=y;
	History[NumSteps++].Steps = Steps;

	if(Steps <= 50) {
		Day2++;
	}

	if(x == TargetX && y == TargetY) {
		printf("Found target at %i steps\n", Steps);
		Found=1;
	}
}

char IsWall(unsigned int x, unsigned int y) {
	unsigned int z = x*x + 3*x + 2*x*y + y + y*y + MagicNumber;
	int b=0;
	for(int i=0;i<32;i++) {
		if(z & 1<<i) {
			b++;
		}
	}
	return b%2 ? '#' : ' ';
}

void Iterate(int idx) {
	if(History[idx].X > 0) {
		AddToState(History[idx].X-1, History[idx].Y, History[idx].Steps+1);
	}
	AddToState(History[idx].X+1, History[idx].Y, History[idx].Steps+1);
	if(History[idx].Y > 0) {
		AddToState(History[idx].X, History[idx].Y-1, History[idx].Steps+1);
	}
	AddToState(History[idx].X, History[idx].Y+1, History[idx].Steps+1);
}

int main(int argc, char *argv[]) {
	memset(Maze, 0, sizeof(Maze));
	Maze[1][1] = ' ';
	AddToState(1, 1, 0);

	for(int i=0; i<NumSteps && !Found; i++) {
		//printf("%i / %lu (%u)\n", i+1, NumSteps, History[i].Steps);
		Iterate(i);
	}

	printf("%u Locations available in 50 steps\n", Day2);

	return 0;
}
