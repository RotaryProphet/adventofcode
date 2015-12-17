#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int TargetQty = 150;

int cmpint(const void *a, const void *b) {
	if(*(int*)a > *(int*)b) {
		return -1;
	} else if(*(int*)a < *(int*)b) {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char buffer[128];
	int valid_combos=0;
	int *sizes=NULL;
	int numsizes=0;
	int *amounts;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		int size=atoi(buffer);
		sizes=realloc(sizes, (numsizes+1) * sizeof(*sizes));
		sizes[numsizes++]=size;
	}

	qsort(sizes, numsizes, sizeof(*sizes), &cmpint);
	amounts = (int*)malloc(numsizes * sizeof(*amounts));

	for(;;) {
		int i;
		int t=0;
		int p=0;

		for(i=0;;i++) {
			if(i==numsizes) {
				goto done;
			}

			amounts[i]++;
			if(amounts[i] * sizes[i] > TargetQty) {
				amounts[i]=0;
				p++;
				continue;
			}
			break;
		}

		if(p>1) {
			for(i=0;i<numsizes;i++) {
				printf("%i,", amounts[i]);
			}
			printf("\b\n");
		}

		for(i=0;i<numsizes;i++) {
			t+=amounts[i] * sizes[i];
			if(t > TargetQty) {
				break;
			}
		}

		if(t == TargetQty) {
			valid_combos++;
		}
	}
done:
	printf("Number of valid combinations is %i\n", valid_combos);
	return 0;
}
