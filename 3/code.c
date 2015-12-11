#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _House {
	int		x;
	int		y;
	int		presents;
	struct _House	*Next;
} House;
House *Houses=NULL;

typedef struct _Santa {
	int		x;
	int		y;
	struct _Santa	*Next;
} Santa;
Santa Santas[2]={
	{0, 0, &Santas[1]},
	{0, 0, &Santas[0]}
};

void present(Santa *s) {
	int x, y;
	House *h;

	x=s->x;
	y=s->y;
	for(h=Houses; h; h=h->Next) {
		if(h->x == x && h->y == y) {
			h->presents++;
			return;
		}
	}

	h=(House*)malloc(sizeof(*h));
	memset(h, 0, sizeof(*h));
	h->x = x;
	h->y = y;
	h->presents = 1;
	h->Next = Houses;
	Houses = h;
}

int main(int argc, char *argv[]) {
	int i=0;
	int c;
	int TotalHouses=0;
	int TotalPresents=0;
	House *h;
	Santa *s=&Santas[0];

	present(s);
	present(s->Next);

	while((c=getchar())>=0) {
		switch(c) {
		case '^':
			s->y++;
			break;
		case '>':
			s->x++;
			break;
		case 'v':
		case 'V':
			s->y--;
			break;
		case '<':
			s->x--;
			break;
		default:
			break;

		}

		present(s);
		i++;
		s=s->Next;
	}

	for(h=Houses; h; h=h->Next) {
		TotalHouses++;
		TotalPresents+=h->presents;
	}
	printf("Total Houses: %i\n", TotalHouses);
	printf("Total Presents: %i\n", TotalPresents);
	return 0;
}
