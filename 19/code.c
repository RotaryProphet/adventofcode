#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *strdup(const char *in);

typedef struct _Replacement {
	char	*From;
	size_t	FromLen;
	char	*To;
	size_t	ToLen;
} Replacement;

Replacement *R=NULL;
int NumR=0;
char *Starting=NULL;
char **Found=NULL;
int NumFound=0;

int AddFound(const char *str) {
	int i;
	for(i=0;i<NumFound;i++) {
		if(!strcmp(str, Found[i])) {
			return 0;
		}
	}
	Found=realloc(Found, (NumFound+1)*sizeof(*Found));
	Found[NumFound++]=strdup(str);
	return 1;
}

void ApplyIterations(const char *From) {
	static char Working[4096];
	int i;

	for(i=0;i<strlen(From);i++) {
		int x;

		memcpy(Working, From, i);
		for(x=0;x<NumR;x++) {
			if(strncmp(&From[i], R[x].From, R[x].FromLen)) {
				continue;
			}
			strncpy(&Working[i], R[x].To, sizeof(Working)-i);
			strncpy(&Working[i+R[x].ToLen], &From[i+R[x].FromLen], sizeof(Working)-(i+R[x].FromLen)-R[x].ToLen);
			//printf("Changing '%s' to '%s' index %i produces '%s'\n", R[x].From, R[x].To, i, Working);
			AddFound(Working);
		}
	}
}

int SortRep(const void *a, const void *b) {
	size_t A=((Replacement*)a)->ToLen - ((Replacement*)a)->FromLen;
	size_t B=((Replacement*)b)->ToLen - ((Replacement*)b)->FromLen;
	if(A > B) {
		return -1;
	} else if(A < B) {
		return 1;
	}
	return 0;
}

int Day2Iter(const char *from, const char *tgt, int Iter) {
	char *Working=malloc(4096);
	int i, r, x;

	for(i=0;from[i];i++) {
		for(r=0;r<NumR;r++) {
			if(strncmp(&from[i], R[r].To, R[r].ToLen)) {
				continue;
			}
			strcpy(&Working[i], R[r].From);
			strcpy(&Working[i+R[r].FromLen], &from[i+R[r].ToLen]);
			if(!strcmp(Working, tgt)) {
				printf("Found '%s' at iteration %i\n", tgt, Iter);
				free(Working);
				return Iter;
			}
			if((x=Day2Iter(Working, tgt, Iter+1))) {
				free(Working);
				return x;
			}
		}
		Working[i]=from[i];
	}
	free(Working);
	return 0;
}


	
int Day2Calc(const char *from) {
	// Sort conditions in greedy order (Most replaced chars first)
	qsort(R, NumR, sizeof(*R), SortRep);
	return Day2Iter(from, "e", 1);
}
		

int Day2(const char *from) {
	int i;
	int NumRn=0;
	int NumAr=0;
	int NumY=0;
	int Symbols=0;
	
	for(i=0;from[i];i++) {
		if(from[i] < 'A' || from[i] > 'Z') {
			continue;
		}
		Symbols++;
		if(!strncmp(&from[i], "Rn", 2)) {
			NumRn++;
		} else if(!strncmp(&from[i], "Ar", 2)) {
			NumAr++;
		} else if(!strncmp(&from[i], "Y", 1)) {
			NumY++;
		}
	}
	printf("%i symbols, %i Rn, %i Ar, %i Y\n", Symbols, NumRn, NumAr, NumY);
	return Symbols - NumRn - NumAr - (2*NumY) - 1;
}

int main(int argc, char *argv[]) {
	static char buffer[1024];
	int r;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		char *c;

		if((c=strchr(buffer, '\n'))) {
			*c=0;
		}

		if(!buffer[0]) {
			continue;
		}

		if((c=strchr(buffer, ' '))) {
			R=realloc(R, (NumR+1) * sizeof(*R));

			*c=0;
			R[NumR].From = strdup(buffer);
			R[NumR].FromLen = strlen(R[NumR].From);
			R[NumR].To = strdup(c+4);
			R[NumR].ToLen = strlen(R[NumR].To);
			NumR++;
		} else {
			Starting = strdup(buffer);
		}
	}

	ApplyIterations(Starting);
	printf("Day 1: Total number after 1 iteration is %i\n", NumFound);

	r=Day2(Starting);
	printf("Day 2: %i iterations to make target molecule\n", r);

	r=Day2Calc(Starting);
	printf("Day 2 math: %i iterations\n", r);
	return 0;
}
	
