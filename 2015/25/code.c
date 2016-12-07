#include <stdio.h>
#include <stdlib.h>

const unsigned int start = 20151125;
const unsigned int mult = 252533;
const unsigned int remain = 33554393;
const unsigned int target_x = 3029;
const unsigned int target_y = 2947;

int main(int argc, char *argv[]) {
	unsigned int x, y;
	unsigned long long lastnum;

	x = 1;
	y = 1;
	lastnum = start;
	for(;;) {
		x++;
		y--;
		if(y == 0) {
			y = x;
			x = 1;
		}
		lastnum = (lastnum * mult) % remain;

		if(x == target_x && y == target_y) {
			printf("Value at [%u,%u] = %llu\n", x, y, lastnum);
			break;
		}
	}
	return 0;
}
