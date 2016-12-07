#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int MaxUnits = 100;

typedef struct _Ingredient {
	char		Name[64];
	int		Capacity;
	int		Durability;
	int		Flavor;
	int		Texture;
	int		Calories;
} Ingredient;

Ingredient *Ingredients=NULL;
size_t NumI=0;

int main(int argc, char *argv[]) {
	char buffer[1024];
	int *amounts;
	int besttot, bestcal;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		char *parts[6], *c;

		parts[0] = buffer;
		
		c=strchr(buffer, ':');
		*c=0;
		c=strchr(c+2, ' ');
		parts[1]=++c; // capacity

		c=strchr(c, ',');
		*c=0;
		c=strchr(c+2, ' ');
		parts[2]=++c; // durability

		c=strchr(c, ',');
		*c=0;
		c=strchr(c+2, ' ');
		parts[3]=++c; // flavor

		c=strchr(c, ',');
		*c=0;
		c=strchr(c+2, ' ');
		parts[4]=++c; // texture
		
		c=strchr(c, ',');
		*c=0;
		c=strchr(c+2, ' ');
		parts[5]=++c;

		if((c=strchr(c, '\n'))) {
			*c=0;
		}

		Ingredients = (Ingredient*)realloc(Ingredients, (NumI+1) * sizeof(*Ingredients));
		strncpy(Ingredients[NumI].Name, parts[0], sizeof(Ingredients[NumI].Name));
		Ingredients[NumI].Capacity=atoi(parts[1]);
		Ingredients[NumI].Durability=atoi(parts[2]);
		Ingredients[NumI].Flavor=atoi(parts[3]);
		Ingredients[NumI].Texture=atoi(parts[4]);
		Ingredients[NumI].Calories=atoi(parts[5]);
		NumI++;
	}

	srand(time(NULL));
	amounts=(int*)malloc(NumI * sizeof(int));
	besttot=0, bestcal=0;
	memset(amounts, 0, sizeof(NumI * sizeof(int)));

	for(;;) {
		int tcap=0, tdur=0, tflav=0, ttex=0, tcal=0, ttot, tnum=0;
		int i;

		for(i=0;;i++) {
			if(i == NumI) {
				goto done;
			} else if(++amounts[i]>MaxUnits) {
				amounts[i]=0;
			} else {
				break;
			}
		}

		for(i=0;i<NumI;i++) {
			tnum+=amounts[i];
			if(tnum > MaxUnits) {
				// Shave 80% off the runtime by checking this before totalling up amounts below
				goto nextloop;
			}
		}
		for(i=0;i<NumI;i++) {
			tcap += amounts[i] * Ingredients[i].Capacity;
			tdur += amounts[i] * Ingredients[i].Durability;
			tflav += amounts[i] * Ingredients[i].Flavor;
			ttex += amounts[i] * Ingredients[i].Texture;
			tcal += amounts[i] * Ingredients[i].Calories;
		}

		if(tcap <= 0 || tdur <= 0 || tflav <= 0 || ttex <= 0) {
			continue;
		}

		ttot = tcap * tdur * tflav * ttex;
		if(ttot > besttot) {
			besttot = ttot;
		}

		if(ttot > bestcal && tcal == 500) {
			bestcal=ttot;
		}
nextloop:
		continue;
	}
done:	
	printf("Best total is %i\n", besttot);
	printf("Best total under 500 calories is %i\n", bestcal);
	return 0;
}
