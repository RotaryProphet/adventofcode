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

int MakeTarget(const char *Target) {
	int Iterations;
	int LastM=0;

	for(Iterations=1;;Iterations++) {
		int i;
		int m=NumFound;

		for(i=LastM;i<m;i++) {
			ApplyIterations(Found[i]);
			printf("\r(%i) - %i/%i", Iterations, i+1, m);
			fflush(stdout);
		}
		for(i=m;i<NumFound;i++) {
			if(!strcmp(Found[i], Target)) {
				return Iterations;
			}
		}
		LastM=m;
		printf("\n");
	}			
	return -1;
}

void Cleanup() {
	int i;
	for(i=0;i<NumFound;i++) {
		free(Found[i]);
		Found[i]=NULL;
	}
	free(Found);
	Found=NULL;
	NumFound=0;
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

	Cleanup();
	AddFound("e");
	r=MakeTarget(Starting);
	printf("Day 2: %i iterations to make target molecule\n", r);
	return 0;
}
	
