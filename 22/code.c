#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
//#define DBG(...) printf(__VA_ARGS__)
#define DBG(...)
#define NUMPLAYERS 2
#define NUMSPELLS 5
#define NUMROUNDS 100

int LowestManaWin = -1;

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

typedef struct _SpellStack {
	Spell			*S[NUMROUNDS];
} SpellStack;

typedef struct _Round {
	Character	Chars[NUMPLAYERS];
	int		ManaSpent;
	Effect		Effects[NUMSPELLS];
	int		RoundNum;
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
	return NULL;
}

void DoEffects(Round *R) {
	Effect *E;
	R->Chars[0].armor=0;
	int i;
	for(i=0;i<NUMSPELLS;i++) {
		E=&R->Effects[i];
		if(E->Rounds > 0) {
			R->Chars[1].hp -= E->S.dmg;
			R->Chars[0].hp = MIN(R->Chars[0].hp + E->S.hp, Starting.hp);
			R->Chars[0].armor += E->S.armor;
			R->Chars[0].mana += E->S.mana;
			E->Rounds--;
		}
	}
}

void PrintSpellStack(SpellStack *S, int Num) {
	int i;
	DBG("Spell stack (%i): ", Num);
	for(i=1;i<=Num;i+=Num) {
		DBG("%s,", S->S[i]->Name);
	}
	DBG("\n");
}

void RunRound(Round *R, SpellStack *Stack) {
	int i;

	if(++R->RoundNum > NUMROUNDS) {
		DBG("Skipping round that took too long\n");
		return;
	}
	DBG("%*s%i: %i(%i) vs %i: ", R->RoundNum*4, "", R->RoundNum, R->Chars[0].hp, R->Chars[0].mana, R->Chars[1].hp);

	//if(LowestManaWin > 0 && R->ManaSpent >= LowestManaWin) {
		//continue;
	//}

	DoEffects(R);
	if(R->Chars[1].hp <= 0) {
		if(LowestManaWin <= 0 || R->ManaSpent < LowestManaWin) {
			printf("New lowest win: %i\n", R->ManaSpent);
			LowestManaWin = R->ManaSpent;
		} else {
			DBG("Won, but not lowest\n");
		}
		return;
	}
	if(R->Chars[0].mana < Spellbook[0].cost) {
		DBG("Died\n");
		return;
	}
	DBG("\n");
	for(i=0;i<sizeof(Spellbook)/sizeof(Spellbook[0]);i++) {
		Round New;
		Spell *S=&Spellbook[i];
		if(EffectActive(R, S)) {
			DBG("%*sSkipping %s because already active\n", R->RoundNum * 4, "", S->Name);
			continue;
		} else if(S->cost > R->Chars[0].mana) {
			DBG("%*sSkipping %s because not enough mana\n", R->RoundNum * 4, "", S->Name);
			continue;
		}

		if(LowestManaWin > 0 && R->ManaSpent + S->cost >= LowestManaWin) {
			DBG("%*sAlready over lowest\n", R->RoundNum * 4, "");
			continue;
		}

		DBG("%*sCasting %s: ", R->RoundNum * 4, "", S->Name);
		Stack->S[R->RoundNum]=S;

		memcpy(&New, R, sizeof(New));
		AddEffect(&New, S);
		New.Chars[0].mana-=S->cost;
		New.ManaSpent+=S->cost;
		DoEffects(&New);
		if(New.Chars[1].hp <= 0) {
			if(LowestManaWin <= 0 || R->ManaSpent < LowestManaWin) {
				printf("New lowest win: %i\n", R->ManaSpent);
				LowestManaWin = R->ManaSpent;
				PrintSpellStack(Stack, R->RoundNum);
			} else {
				DBG("Won, but not lowest\n");
			}
			continue;
		}
	
		New.Chars[0].hp -= New.Chars[1].dmg - New.Chars[0].armor;
		if(New.Chars[0].hp <= 0) {
			DBG("Lost\n");
			continue;
		}
		DBG("\n");
		RunRound(&New, Stack);
	}
}

int main(int argc, char *argv[]) {
	Round *R;
	SpellStack Stack;

	R=malloc(sizeof(*R));
	memset(R, 0, sizeof(*R));
	memcpy(&R->Chars[0], &Starting, sizeof(Starting));
	memcpy(&R->Chars[1], &Boss, sizeof(Boss));
	memset(&Stack, 0, sizeof(Stack));

	RunRound(R, &Stack);
	printf("Lowest mana win: %i\n", LowestManaWin);		
}

