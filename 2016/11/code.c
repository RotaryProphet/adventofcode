#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUMFLOORS 4
#define NUMITEMS 10

typedef struct _Floor {
	int NumItems;
	char Items[NUMITEMS][3];
} Floor;

typedef struct _State {
	int Steps;
	int Elevator;
	Floor Floors[NUMFLOORS];
} State;

const State InitState = {
	0, 0, {
		{4,{"SG","SM","PG","PM"}},
		{5,{"TG","RG","RM","CG","CM"}},
		{1,{"TM"}},
		{0,{}}
	}
};

State **Queue=NULL;
int QueueSize=0;
int QueuePos=0;

void AddToQueue(const State *S) {
	if(QueuePos == QueueSize) {
		Queue=(State**)realloc(Queue, (QueueSize+64)*sizeof(*Queue));
		QueueSize+=64;
	}
	Queue[QueuePos]=(State*)malloc(sizeof(**Queue));
	memcpy(Queue[QueuePos], S, sizeof(*S));
	QueuePos++;
}

void DumpState(State *S) {
	printf("Step %i\n", S->Steps);
	for(int i=NUMFLOORS-1;i>=0;i--) {
		printf("%s %i|%i|", i == S->Elevator ? "E" : " ", i, S->Floors[i].NumItems);
		for(int j=0;j<S->Floors[i].NumItems;j++) {
			printf("%s,", S->Floors[i].Items[j]);
		}
		puts("");
	}
}

bool HasItem(Floor *F, char *Item) {
	for(int i=0;i<F->NumItems;i++) {
		if(!strncmp(Item, F->Items[i], 2)) {
			return 1;
		}
	}
	return 0;
}

bool IsValid(State *S) {
	char Item[3]=" G";
	for(int i=0;i<NUMFLOORS;i++) {
		Floor *F=&S->Floors[i];
		int Gens=0;
		for(int j=0;j<F->NumItems;j++) {
			if(F->Items[j][1] == 'G') {
				Gens++;
			}
		}
		if(Gens) {
			for(int j=0;j<F->NumItems;j++) {
				if(F->Items[j][1] == 'M') {
					Item[0] = F->Items[j][0];
					if(!HasItem(F, Item)) {
						printf("\n\tHas %s but not %s... ", F->Items[j], Item);
						return 0;
					}
				}
			}
		}
	}
	return 1;
}

bool InQueue(State *S) {
	for(int i=0;i<QueuePos;i++) {
		for(int f=0;f<NUMFLOORS;f++) {
			for(int j=0;j<S->Floors[i].NumItems;j++) {
				if(!HasItem( &(Queue[i]->Floors[f]), S->Floors[f].Items[j])) {
					goto NextQueue;	
				}
			}
		}
		return 1;
NextQueue:
		if(1)
		;
	}
	return 0;
}

void TryIteration(const State *S, int idx1, int idx2, int ToFloor) {
	State N;
	int f = S->Elevator;

	memcpy(&N, S, sizeof(N));
	N.Elevator = ToFloor;
	N.Steps++;

	printf("Moving %i (%s), %i (%s) from %i to %i... ", idx1, N.Floors[f].Items[idx1],
			idx2, (idx2 >= 0 ? N.Floors[f].Items[idx2] : " "), f, ToFloor);
	strcpy(N.Floors[ToFloor].Items[ N.Floors[ToFloor].NumItems++ ], N.Floors[f].Items[idx1]);
	if(idx2 >= 0) {
		strcpy(N.Floors[ToFloor].Items[ N.Floors[ToFloor].NumItems++ ], N.Floors[f].Items[idx2]);
		memmove(N.Floors[f].Items[idx2], N.Floors[f].Items[idx2+1],
			(N.Floors[f].NumItems-idx2-1)*sizeof(int));
		N.Floors[f].NumItems--;
	}
	memmove(N.Floors[f].Items[idx1], N.Floors[f].Items[idx1+1],
		(N.Floors[f].NumItems-idx1-1)*sizeof(int));
	N.Floors[f].NumItems--;


	//puts("");
	//DumpState(&N);
	if(!IsValid(&N)) {
		printf("Not valid\n");
	} else if(InQueue(&N)) {
		printf("In queue\n");
	} else {
		printf("OK\n");
		if(N.Floors[ NUMFLOORS-1 ].NumItems == NUMITEMS) {
			printf("Number of steps: %i\n", N.Steps);
			exit(0);
		}
		AddToQueue(&N);
	}
}

void Step(const State *S) {
	for(int i=0; i<S->Floors[ S->Elevator ].NumItems; i++) {
		if(S->Elevator > 0) {
			TryIteration(S, i, -1, S->Elevator-1);
		}
		if(S->Elevator < NUMFLOORS-1) {
			TryIteration(S, i, -1, S->Elevator+1);
		}

		for(int j=i+1; j<S->Floors[ S->Elevator ].NumItems; j++) {
			if(S->Elevator > 0) {
				TryIteration(S, i, j, S->Elevator-1);
			}
			if(S->Elevator < NUMFLOORS-1) {
				TryIteration(S, i, j, S->Elevator+1);
			}
		}
	}
}


int main(int argc, char *argv[]) {
	AddToQueue(&InitState);
	for(int i=0;i<QueuePos;i++) {
		printf("At %i of %i\n", i+1, QueuePos);
		if(Queue[i]->Steps > 20) {
			printf("Stopping at 20 steps\n");
			break;
		}
		Step(Queue[i]);
	}	
	return 0;
}

