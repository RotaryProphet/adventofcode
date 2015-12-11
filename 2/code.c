#include <stdio.h>
#include <stdlib.h>

int comp (const void *a, const void *b) {
	if(*(int*)a > *(int*)b) {
		return 1;
	} else if(*(int*)a < *(int*)b) {
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	int sides[3];
	int total=0;
	int ribbon=0;
	int r;

	while(scanf("%dx%dx%d", &sides[0], &sides[1], &sides[2])==3) {
		qsort(sides, sizeof(sides)/sizeof(*sides), sizeof(*sides), &comp);
		r = (3 * sides[0] * sides[1]) + (2 * sides[1] * sides[2]) + (2 * sides[0] * sides[2]);
		total += r;

		r = (2 * sides[0]) + (2 * sides[1]) + (sides[0] * sides[1] * sides[2]);
		ribbon += r;
	}
	printf("Requires %i square feet\n", total);
	printf("Requires %i feet of ribbon\n", ribbon);
	return 0;
}
