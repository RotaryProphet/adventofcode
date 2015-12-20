#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

const int target=29000000;
int main() {
	unsigned int house, elf;
	int highest=0;

	for(house=1;;house++) {
		int presents=house * 10 + 10;
		for(elf=2;elf<house/2;elf++) {
			if(house % elf == 0) {
				presents += elf * 10;
			}
		}
		if(presents >= target) {
			printf("House %i is over (%i)\n", house, presents);
			break;
		} else if(presents > highest) {
			printf("New highest: %i @ %i\n", presents, house);
			highest=presents;
		}
	}
	return 0;
}
