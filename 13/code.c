#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NameLen 64

typedef struct _Happiness {
	char	Name1[NameLen];
	char	Name2[NameLen];
	int	Happiness;
} Happiness;

Happiness *H=NULL;
size_t NumH=0;

typedef struct _NameList {
	char	Name[NameLen];
} NameList;

NameList *Names;
size_t NumNames=0;

int GetHappiness(const char *Name1, const char *Name2) {
	size_t i;
	int r=0;

	for(i=0;i<NumH;i++) {
		if(	(!strcmp(Name1, H[i].Name1) || !strcmp(Name1, H[i].Name2))
		     && (!strcmp(Name2, H[i].Name1) || !strcmp(Name2, H[i].Name2))
		) {
			r+=H[i].Happiness;
		}
	}
	return r;
}

void InsertNameList(const char *Name) {
	size_t i;

	for(i=0;i<NumNames;i++) {
		if(!strcmp(Name, Names[i].Name)) {
			return;
		}
	}
	
	Names=(NameList*)realloc(Names, (NumNames+1) * sizeof(*Names));
	strncpy(Names[NumNames].Name, Name, sizeof(Names->Name));
	NumNames++;
}

void InsertHappiness(const char *Name1, const char *Name2, int Happiness) {
	H = (struct _Happiness*)realloc(H, (NumH+1) * sizeof(*H));
	strncpy(H[NumH].Name1, Name1, sizeof(H->Name1));
	strncpy(H[NumH].Name2, Name2, sizeof(H->Name2));
	H[NumH].Happiness = Happiness;
	NumH++;
}

int CalcHappiness(NameList *List, size_t Num) {
	int r=0;
	int i;

	for(i=0; i<Num; i++) {
		int n = (i == Num-1 ? 0 : i+1);
		r+=GetHappiness(List[i].Name, List[n].Name);
	}
	return r;
}

int main(int argc, char *argv[]) {
	char buffer[1024];
	char *parts[64];
	NameList *Best, *Working;
	int BestH=0;
	int i;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		int pid=0;
		char *p;
		int happiness;

		if((p=strchr(buffer, '.'))) {
			*p=0;
		} else if((p=strchr(buffer, '\n'))) {
			*p=0;
		}
		p=buffer;
		parts[pid++]=buffer;
		while((p=strchr(p, ' '))) {
			*p=0;
			parts[pid++]=++p;
		}

		InsertNameList(parts[0]);
		InsertNameList(parts[10]);
		happiness = atoi(parts[3]);

		if(!strcmp(parts[2], "lose")) {
			happiness *= -1;
		}

		printf("%s\t%s\t%i\n", parts[0], parts[10], happiness);
		InsertHappiness(parts[0], parts[10], happiness);
	}
	
	Best=(NameList*)malloc((NumNames+1) * sizeof(*Best));
	Working=(NameList*)malloc((NumNames+1) * sizeof(*Working));
	memcpy(Working, Names, NumNames * sizeof(*Working));
	srand(time(NULL));

	for(i=0;i<20000;i++) {
		NameList Tmp;
		int swap1 = rand() % NumNames;
		int swap2;
		int h;
		do {
			swap2 = rand() % NumNames;
		} while(swap1 == swap2);
		memcpy(&Tmp, &Working[swap1], sizeof(Tmp));
		memcpy(&Working[swap1], &Working[swap2], sizeof(Tmp));
		memcpy(&Working[swap2], &Tmp, sizeof(Tmp));
		h=CalcHappiness(Working, NumNames);
		if(h > BestH) {
			BestH=h;
			memcpy(Best, Working, NumNames * sizeof(*Best));
		}
	}

	printf("Day 1 Best happiness is %i\n", BestH);
	BestH=0;

	strcpy(&Working[NumNames++], "Me");
	for(i=0;i<20000;i++) {
		NameList Tmp;
                int swap1 = rand() % NumNames;
		int swap2;
		int h;
		do {
			swap2 = rand() % NumNames;
		} while(swap1 == swap2);
		memcpy(&Tmp, &Working[swap1], sizeof(Tmp));
		memcpy(&Working[swap1], &Working[swap2], sizeof(Tmp));
		memcpy(&Working[swap2], &Tmp, sizeof(Tmp));
		h=CalcHappiness(Working, NumNames);
		if(h > BestH) {
			BestH=h;
			memcpy(Best, Working, NumNames * sizeof(*Best));
		}
	}
	
	printf("Day 2 Best happiness is %i\n", BestH);

	return 0;
}
