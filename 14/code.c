#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int TargetTime = 2503;

//#define DBG(...) printf(__VA_ARGS__)
#define DBG(...)

#define FLYING 0
#define RESTING 1
typedef struct _Runners {
	char		Name[64];
	int		Speed;
	int		OnTime;
	int		OffTime;
	int		Status;
	int		Counter;
	int		Distance;
	int		Points;
} Runners;

Runners *Deer=NULL;

int NumDeer=0;

int main(int argc, char *argv[]) {
	char buffer[1024];
	char *parts[32];
	int seconds;
	int i;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		int pc=1;
		
		parts[0]=buffer;
		for(i=1;buffer[i];i++) {
			if(buffer[i] == ' ') {
				buffer[i]=0;
				parts[pc++]=&buffer[i+1];
			}
		}
		
		Deer = (Runners*)realloc(Deer, (NumDeer+1) * sizeof(*Deer));
		memset(&Deer[NumDeer], 0, sizeof(*Deer));
		strncpy(Deer[NumDeer].Name, parts[0], sizeof(Deer->Name));
		Deer[NumDeer].Speed = atoi(parts[3]);
		Deer[NumDeer].OnTime = atoi(parts[6]);
		Deer[NumDeer].OffTime = atoi(parts[13]);
		NumDeer++;
	}

	for(seconds=1;seconds<=TargetTime;seconds++) {
		int LongestD=0;
		DBG("After %i seconds...\n", seconds);
		for(i=0;i<NumDeer;i++) {
			Runners *d=&Deer[i];
			if(d->Status == FLYING) {
				if(d->Counter >= d->OnTime) {
					DBG("%s stops flying\n", d->Name);
					d->Status = RESTING;
					d->Counter=1;
				} else {
					d->Distance += d->Speed;
					d->Counter++;
					DBG("%s flies %i km (Total of %i)\n", d->Name, d->Speed, d->Distance);
				}
			} else {
				if(d->Counter >= d->OffTime) {
					d->Status = FLYING;
					d->Distance += d->Speed;
					d->Counter = 1;
					DBG("%s STARTS flying %i km (Total of %i)\n", d->Name, d->Speed, d->Distance);
				} else {
					DBG("%s rests\n", d->Name);
					d->Counter++;
				}
			}
			if(d->Distance > LongestD) {
				LongestD = d->Distance;
			}
		}
		for(i=0;i<NumDeer;i++) {
			if(Deer[i].Distance == LongestD) {
				Deer[i].Points++;
			}
		}
	}

	for(i=0;i<NumDeer;i++) {
		printf("%s has flown %i km with %i points\n", Deer[i].Name, Deer[i].Distance, Deer[i].Points);
	}
	return 0;
}
