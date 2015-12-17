#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

const int TargetQty = 150;

int cmpint(const void *a, const void *b) {
	if(*(int*)a < *(int*)b) {
		return -1;
	} else if(*(int*)a > *(int*)b) {
		return 1;
	}
	return 0;
}

char *DescAmounts(uint64_t bits, int *sizes, int numsizes) {
	static char buffer[1024];
	size_t pos=0;
	int i;

	for(i=0;i<numsizes;i++) {
		if(bits & (1<<i)) {
			pos+=snprintf(&buffer[pos], sizeof(buffer)-pos, "%i,", sizes[i]);
		}
	}
	if(pos==0) {
		snprintf(buffer, sizeof(buffer), "Empty");
	} else {
		buffer[pos-1]=0;
	}
	return buffer;
}

int main(int argc, char *argv[]) {
	char buffer[128];
	int valid_combos=0;
	int *sizes=NULL;
	int numsizes=0;
	uint64_t bits;
	int i;
	int minqty;
	int minsize=0;
	

	while(fgets(buffer, sizeof(buffer), stdin)) {
		int size=atoi(buffer);
		sizes=realloc(sizes, (numsizes+1) * sizeof(*sizes));
		sizes[numsizes++]=size;
	}

	qsort(sizes, numsizes, sizeof(*sizes), &cmpint);
	
	printf("Set is: %s\n", DescAmounts(-1, sizes, numsizes));

	minqty = numsizes+1;

	for(bits=0;!(bits & (1<<numsizes));bits++) {
		int t=0;
		int qty=0;
		for(i=0;i<numsizes;i++) {
			if(bits & (1<<i)) {
				t+=sizes[i];
				qty++;
			}
		}

		if(t == TargetQty) {
			//printf("good set %s=%i\n", DescAmounts(bits, sizes, numsizes), t);
			valid_combos++;
			if(qty < minqty) {
				minqty=qty;
				minsize=1;
			} else if(qty == minqty) {
				minsize++;
			}
		}
	}

	printf("Number of valid combinations is %i\n", valid_combos);
	printf("Minimum number of containers is %i, %i combinations\n", minqty, minsize);
	return 0;
}
