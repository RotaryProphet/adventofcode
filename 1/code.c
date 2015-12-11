#include <stdio.h>

int main(int argc, char *argv[]) {
	int i;
	int d=0;
	int u=0;
	int fb=0;
	int c;

	for(i=0;(c = getchar()) > 0;i++) {
		if(c == '(') {
			u++;
		} else if(c == ')') {
			d++;
		}

		if(!fb && 0+u-d == -1) {
			printf("First basement at index %i, pos %i\n", i, i+1);
			fb=1;
		}
	}
	printf("Down\t%i\n", d);
	printf("Up\t%i\n", u);
	printf("Floor\t%i\n", 0+u-d);
	return 0;
}
