#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

#define target 29000000
int houses[target/10];

int main() {
	memset(houses, 0, sizeof(houses));
	int i,j,s;
	for(i=1;i<target/10;i++) {
		for(j=i;j<target/10;j+=i) {
			houses[j]+=i*10;
		}
	}

	for(j=1;j<target/10;j++) {
		if(houses[j] >= target) {
			printf("Day 1: Found target at house %i (%i)\n", j, houses[j]);
			break;
		}
	}

	memset(houses, 0, sizeof(houses));
	for(i=1;i<target/10;i++) {
		for(j=i, s=0; s<50 && j<target/10;j+=i, s++) {
			houses[j]+=i*11;
			//printf("Elf %i delivering to house %i (%i)\n", i, j, houses[j]);
		}
	}

	for(j=1;j<target/10;j++) {
		if(houses[j] >= target) {
			printf("Day 2: Found target at house %i (%i)\n", j, houses[j]);
			break;
		}
	}

		
	return 0;
}
