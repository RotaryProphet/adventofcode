#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

char *strdup(const char *);

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
//#define DBG(...) printf(__VA_ARGS__)
#define DBG(...)
#define NUMPLAYERS 2
#define NUMSPELLS 5
#define NUMROUNDS 100

int LowestManaWin = -1;
char *LowestLog = NULL;
typedef struct _Character {
	int	hp;
	int	dmg;
	int	armor;
	int	mana;
} Character;

typedef struct _Spell {
	char	Name[32];
	int	cost;
	int	hp;
	int	dmg;
	int	armor;
	int	mana;
	int	rounds;
} Spell;

typedef struct _Effect {
	Spell		S;
	int		Rounds;
} Effect;

typedef struct _DebugLog {
	char		*Log;
	size_t		LogPos;
	size_t		LogMax;
} DebugLog;


typedef struct _Round {
	Character	Chars[NUMPLAYERS];
	int		ManaSpent;
	Effect		Effects[NUMSPELLS];
	int		RoundNum;
	int		HardMode;
	DebugLog	Log;
} Round;

Character Boss={71,10,0,0};
Character Starting={50,0,0,500};

Spell Spellbook[NUMSPELLS]={
	{"Magic Missile",	53,	0,	4,	0,	0,	1},
	{"Drain",		73,	2,	2,	0,	0,	1},
	{"Shield",		113,	0,	0,	7,	0,	6},
	{"Poison",		173,	0,	3,	0,	0,	6},
	{"Recharge",		229,	0,	0,	0,	101,	5},
};

void Debug(Round *R, const char *fmt, ...) {
	size_t len;
	va_list args;
	va_start(args, fmt);

	if(!R->Log.Log) {
		R->Log.Log = malloc(40960);
		R->Log.LogPos=0;
		R->Log.LogMax=40960;
	}

	for(;;) {
		len = vsnprintf(&(R->Log.Log[R->Log.LogPos]), R->Log.LogMax - R->Log.LogPos,
				fmt, args);
		if(R->Log.LogPos + len >= R->Log.LogMax) {
			if(!(R->Log.Log = realloc(R->Log.Log, R->Log.LogMax + 4096))) {
				fprintf(stderr, "Error reallocing log\n");
				exit(1);
			}
			printf("Enlarged buffer to %lu\n", R->Log.LogMax + 4096);
			R->Log.LogMax+=4096;
		} else {
			R->Log.LogPos+=len;
			return;
		}
	}
}

void Won(Round *R) {
	if(LowestManaWin <= 0 || R->ManaSpent < LowestManaWin) {
		printf("New lowest win: %i\n", R->ManaSpent);
		Debug(R, "Won\n");
		if(LowestLog) {
			free(LowestLog);
		}
		LowestLog=strdup(R->Log.Log);
		LowestManaWin = R->ManaSpent;
	}
}

int EffectActive(Round *R, Spell *S) {
	int i;
	for(i=0;i<NUMSPELLS;i++) {
		if(!strcmp(R->Effects[i].S.Name, S->Name) && R->Effects[i].Rounds > 0) {
			return 1;
		}
	}
	return 0;
}

Effect *AddEffect(Round *R, Spell *S) {
	int i;

	for(i=0;i<NUMSPELLS;i++) {
		if(R->Effects[i].Rounds <= 0 || !R->Effects[i].S.Name[0]) {
			memcpy(&R->Effects[i].S, S, sizeof(*S));
			R->Effects[i].Rounds = S->rounds;
			return &R->Effects[i];
		}
	}
	fprintf(stderr, "Unable to find empty Effect position!\n");
	exit(1);
	return NULL;
}

void DoEffects(Round *R) {
	R->Chars[0].armor=0;
	int i;
	for(i=0;i<NUMSPELLS;i++) {
		Effect *E=&R->Effects[i];
		if(E->Rounds > 0) {
			if(E->S.dmg) {
				R->Chars[1].hp -= E->S.dmg;
				Debug(R, "%s did %i dmg to boss (%i)\n", E->S.Name,
					E->S.dmg, R->Chars[1].hp);
			}
			if(E->S.hp) {
				int h=MIN(Starting.hp - R->Chars[0].hp, E->S.hp);
				R->Chars[0].hp += h;
				Debug(R, "%s healed %i hp (%i)\n", E->S.Name, h, R->Chars[0].hp);
			}
			if(E->S.armor) {
				R->Chars[0].armor += E->S.armor;
				Debug(R, "%s armor increased by %i (%i)\n", E->S.Name, E->S.armor,
					R->Chars[0].armor);
			}
			if(E->S.mana) {
				R->Chars[0].mana += E->S.mana;
				Debug(R, "%s increased mana by %i (%i)\n", E->S.Name, E->S.mana,
					R->Chars[0].mana);
			}
			E->Rounds--;
			Debug(R, "%s has %i rounds left\n", E->S.Name, E->Rounds);
		}
	}
}

void RunRound(Round *R) {
	int i;

	if(++R->RoundNum > NUMROUNDS) {
		DBG("Skipping round that took too long\n");
		return;
	}
	Debug(R, "%i.1: %i(%i) vs %i: ", R->RoundNum, R->Chars[0].hp, R->Chars[0].mana,
		R->Chars[1].hp);

	if(R->HardMode) {
		R->Chars[0].hp--;
		Debug(R, "Hard mode, 1 damage to player (%i)\n", R->Chars[0].hp);
		if(R->Chars[0].hp <= 0) {
			return;
		}
	}

	//if(LowestManaWin > 0 && R->ManaSpent >= LowestManaWin) {
		//continue;
	//}

	DoEffects(R);
	if(R->Chars[1].hp <= 0) {
		Won(R);
		return;
	}
	for(i=0;i<sizeof(Spellbook)/sizeof(Spellbook[0]);i++) {
		Round *New;
		Spell *S=&Spellbook[i];
		int d;
		if(EffectActive(R, S)) {
			continue;
		} else if(S->cost > R->Chars[0].mana) {
			continue;
		}

		if(LowestManaWin > 0 && R->ManaSpent + S->cost >= LowestManaWin) {
			continue;
		}

		New=malloc(sizeof(*New));
		
		memcpy(New, R, sizeof(*New));
		New->Log.Log = malloc(R->Log.LogMax);
		memcpy(New->Log.Log, R->Log.Log, R->Log.LogMax);
		//Debug(&New, R->Log.Log); // Copy logs to new round
		
		New->Chars[0].mana-=S->cost;
		Debug(New, "Casting spell %s costs %i mana (%i)\n", S->Name, S->cost,
			New->Chars[0].mana);
		AddEffect(New, S);
		New->ManaSpent+=S->cost;
		Debug(New, "Total mana spent: %i\n", New->ManaSpent);

		Debug(New, "%i.2: %i(%i) vs %i: ", New->RoundNum, New->Chars[0].hp,
			New->Chars[0].mana, New->Chars[0].hp);
		
		DoEffects(New);
		if(New->Chars[1].hp <= 0) {
			Won(New);
			free(New->Log.Log);
			free(New);
			continue;
		}

		d = MAX(1, New->Chars[1].dmg - New->Chars[0].armor);
		New->Chars[0].hp -= d;
		Debug(New, "Boss hit player for %i damage (%i)\n", d, New->Chars[0].hp);
		if(New->Chars[0].hp <= 0) {
			free(New->Log.Log);
			free(New);
			continue;
		}
		RunRound(New);
		free(New->Log.Log);
		free(New);
	}
}

int main(int argc, char *argv[]) {
	Round *R=malloc(sizeof(*R));

	memset(R, 0, sizeof(*R));
	memcpy(&R->Chars[0], &Starting, sizeof(Starting));
	memcpy(&R->Chars[1], &Boss, sizeof(Boss));

	RunRound(R);
	free(R->Log.Log);
	//printf(LowestLog);
	printf("Day 1 Lowest mana win: %i\n", LowestManaWin);	

	LowestManaWin=-1;
	memset(R, 0, sizeof(*R));
	memcpy(&R->Chars[0], &Starting, sizeof(Starting));
	memcpy(&R->Chars[1], &Boss, sizeof(Boss));
	R->HardMode=1;
	RunRound(R);
	free(R->Log.Log);
	//printf(LowestLog);
	printf("Day 2 Lowest mana win: %i\n", LowestManaWin);	

	free(R);
}

