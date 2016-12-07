#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)
typedef struct _Character {
	int	hp;
	int	dmg;
	int	armor;
} Character;
typedef struct _Item {
	char	Name[32];
	int	cost;
	int	dmg;
	int	armor;
	char	type;
} Item;
typedef struct _Loadout {
	Item	*Weapon;
	Item	*Armor;
	Item	*Ring1;
	Item	*Ring2;
} Loadout;

Character Boss={100,8,2};
Item Store[]={
	{"Dagger",	8,	4,	0,	'w'},
	{"Shortsword",	10,	5,	0,	'w'},
	{"Warhammer",	25,	6,	0,	'w'},
	{"Longsword",	40,	7,	0,	'w'},
	{"Greataxe",	74,	8,	0,	'w'},

	//Armor:,	Cost  Damage  Armor
	{"Bare",	0,	0,	0,	'a'},
	{"Leather",	13,	0,	1,	'a'},
	{"Chainmail",	31,	0,	2,	'a'},
	{"Splintmail",	53,	0,	3,	'a'},
	{"Bandedmail",	75,	0,	4,	'a'},
	{"Platemail",	102,	0,	5,	'a'},

	//Rings:,	Cost  Damage  Armor
	{"NoRing1",	0,	0,	0,	'r'},
	{"NoRing2",	0,	0,	0,	'r'},
	{"Damage +1",	25,	1,	0,	'r'},
	{"Damage +2",	50,	2,	0,	'r'},
	{"Damage +3",	100,	3,	0,	'r'},
	{"Defense +1",	20,	0,	1,	'r'},
	{"Defense +2",	40,	0,	2,	'r'},
	{"Defense +3",	80,	0,	3,	'r'},
	{"",		0,	0,	0,	0}
};

int runsim(Character *Player, Character *Vs) {
	int round;
	int php=Player->hp;
	int bhp=Vs->hp;

	for(round=1;;round++) {
		bhp-=MAX(1, Player->dmg - Vs->armor);
		if(bhp<=0) {
			break;
		}

		php-=MAX(1, Vs->dmg - Player->armor);
		if(php<=0) {
			break;
		}
	}
	return php;
}

int checkload(Character *Player, Loadout *Load) {
	Player->dmg = Load->Weapon->dmg + Load->Armor->dmg + Load->Ring1->dmg + Load->Ring2->dmg;
	Player->armor = Load->Weapon->armor + Load->Armor->armor + Load->Ring1->armor + Load->Ring2->armor;
	Player->hp = 100;
	return Load->Weapon->cost + Load->Armor->cost + Load->Ring1->cost + Load->Ring2->cost;
}

char *descload(Loadout *Load) {
	static char buffer[1024];
	snprintf(buffer, sizeof(buffer), "%s, %s, %s, %s", Load->Weapon->Name, Load->Armor->Name,
		Load->Ring1->Name, Load->Ring2->Name);
	return buffer;
}

int main(int argc, char *argv[]) {
	Loadout L, B, W;
	Character P;
	int w, a, r1, r2;
	int res, cost;
	int bestcost = -1;
	int bestday2 = -1;

	for(w=0;w<sizeof(Store)/sizeof(Store[0]);w++) {
		if(Store[w].type != 'w') {
			continue;
		}
		L.Weapon = &Store[w];

		for(a=0;a<sizeof(Store)/sizeof(Store[0]);a++) {
			if(Store[a].type != 'a') {
				continue;
			}
			L.Armor = &Store[a];

			for(r1=0;r1<sizeof(Store)/sizeof(Store[0]);r1++) {
				if(Store[r1].type != 'r') {
					continue;
				}
				L.Ring1 = &Store[r1];

				for(r2=0;r2<sizeof(Store)/sizeof(Store[0]);r2++) {
					if(Store[r2].type != 'r' || r1 == r2) {
						continue;
					}
					L.Ring2 = &Store[r2];

					cost = checkload(&P, &L);
					res = runsim(&P, &Boss);
					if(res > 0 && (bestcost == -1 || cost < bestcost)) {
						bestcost = cost;
						memcpy(&B, &L, sizeof(B));
					}
					if(res <= 0 && (bestday2 == -1 || cost > bestday2)) {
						bestday2 = cost;
						memcpy(&W, &L, sizeof(W));
					}
				}
			}
		}
	}
						
	printf("Day 1: Min cost is %i (%s)\n", bestcost, descload(&B));
	printf("Day 2: Worst cost is %i (%s)\n", bestday2, descload(&W));

}
