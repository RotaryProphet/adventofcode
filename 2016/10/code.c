#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct _Bot {
	int Num;
	int Chips[2];
	int NumChips;
	int HighTo;
	bool HighToOut;
	int LowTo;
	bool LowToOut;
} Bot;

Bot *Bots=NULL;
size_t NumBots=0;

void CheckBot(int Num) {
	if(Num >= NumBots) {
		Bots=realloc(Bots, (Num+1)*sizeof(*Bots));
		memset(&Bots[NumBots], 0, (Num+1-NumBots)*sizeof(*Bots));
		NumBots=Num+1;
	}

	if(Bots[Num].Num == 0) {
		Bots[Num].Num = Num;
	}
}

bool HasTwo(int Bot) {
	return Bots[Bot].NumChips == 2 ? 1 : 0;
}

int GetLow(int Bot) {
	int v;
	if(!HasTwo(Bot)) {
		v=Bots[Bot].Chips[0];
	} else if(Bots[Bot].Chips[0] > Bots[Bot].Chips[1]) {
		v=Bots[Bot].Chips[1];
	} else {
		v=Bots[Bot].Chips[0];
		Bots[Bot].Chips[0] = Bots[Bot].Chips[1];
	}
	Bots[Bot].NumChips--;
	return v;
}

int GetHigh(int Bot) {
	int v;
	if(!HasTwo(Bot)) {
		v=Bots[Bot].Chips[0];
	} else if(Bots[Bot].Chips[0] < Bots[Bot].Chips[1]) {
		v=Bots[Bot].Chips[1];
	} else {
		v=Bots[Bot].Chips[0];
		Bots[Bot].Chips[0] = Bots[Bot].Chips[1];
	}
	Bots[Bot].NumChips--;
	return v;
}


bool GiveToBot(int Bot, int Val) {
	if(HasTwo(Bot)) {
		printf("Error: Trying to give value %i to bot %i already has 2 chips\n", Val, Bot);
		exit(1);
		return 0;
	}
	Bots[Bot].Chips[ Bots[Bot].NumChips++ ] = Val;
	return 1;
}

int main(int argc, char *argv[]) {
	char buf[1024]="";
	char *c;
	int b, v, l, h;
	int line=0;
	int outs[3];

	while(fgets(buf, sizeof(buf), stdin)) {
		char *Ptrs[32];
		int NPtrs=0;

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

		if(!strcmp(Ptrs[0], "value")) {
			b=atoi(Ptrs[5]);
			v=atoi(Ptrs[1]);
			CheckBot(b);
			if(!GiveToBot(b, v)) {
				printf("Error: During script bot %i has two already\n", b);
			}/* else {
				printf("Value %i to bot %i\n", v, b);
			}*/
		} else if(!strcmp(Ptrs[0], "bot")) {
			b=atoi(Ptrs[1]);
			l=atoi(Ptrs[6]);
			h=atoi(Ptrs[11]);
			CheckBot(b);
			Bots[b].HighTo=h;
			Bots[b].LowTo=l;
			if(!strcmp(Ptrs[5], "output")) {
				Bots[b].LowToOut=1;
			}
			if(!strcmp(Ptrs[10], "output")) {
				Bots[b].HighToOut=1;
			}
		} else {
			printf("Unknown line %i\n", line);
		}
		line++;
	}

	for(v=1;v;) {
		v=0;
		for(int i=0;i<NumBots;i++) {
			if(!HasTwo(i)) {
				//printf("Bot %i doesn't have 2 (%i)\n", i, Bots[i].NumChips);
				continue;
			}
			printf("Has two: %i\n", i);
			if(	(!Bots[i].LowToOut && HasTwo( Bots[i].LowTo )) ||
				(!Bots[i].HighToOut && HasTwo( Bots[i].HighTo ))
			) {
				printf("Bot %i trying to handoff to full bot\n", i);
				continue;
			}

			h=GetHigh(i);
			l=GetLow(i);
			if(h == 61 && l == 17) {
				printf("Bot %i comparing 61 and 17!\n", i);
			}
			if(!Bots[i].LowToOut) {
				printf("Bot %i giving low %i to %i\n", i, l, Bots[i].LowTo);
				GiveToBot( Bots[i].LowTo, l);
			} else {
				if(Bots[i].LowTo < 3) {
                                	outs[Bots[i].LowTo]=l;
				}
				printf("Bot %i giving low %i to output %i\n", i, l, Bots[i].LowTo);
			}
			if(!Bots[i].HighToOut) {
				printf("Bot %i giving high %i to %i\n", i, h, Bots[i].HighTo);
				GiveToBot( Bots[i].HighTo, h);
			} else {
				if(Bots[i].HighTo < 3) {
					outs[Bots[i].HighTo]=h;
				}
				printf("Bot %i giving high %i to ouput %i\n", i, h, Bots[i].HighTo);
			}
			v++;
		}
		printf("Loop\n");
	}
	printf("Day 2: %i\n", outs[0] * outs[1] * outs[2]);
	return 0;
}












