#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int n=0, t=0, i=0, *nums=NULL, maxnums=0;
	int a, b, c;
	char buf[1024];

	while(fgets(buf, sizeof(buf), stdin)) {
		a = b = c =0;
		scanf(" %i %i %i", &a, &b, &c);
		if(!a) {
			continue;
		}

		if(i + 9 >= maxnums) {
			nums = realloc(nums, (maxnums+100)*sizeof(int));
			maxnums+=100;
		}
		nums[i++]=a;
		nums[i++]=b;
		nums[i++]=c;
		//printf("%i,%i,%i\t", a, b, c);
		if(	a + b > c
		   &&	a + c > b
		   &&	b + c > a
		) {
			//printf("in");
			n++;
		}
		t++;
		//printf("valid\n");
	}
	printf("%i valid of %i\n", n, t);

	n=0;
	t=0;
	maxnums=i;
	for(int j=0;j+8<maxnums;j+=9) {
		for(i=j;i<j+3;i++) {
			int a=nums[i];
			int b=nums[i+3];
			int c=nums[i+6];
			t++;
			if(     a + b > c
		   	&&	a + c > b
		   	&&	b + c > a
			) {
				n++;
			}
		}
	}
	printf("Part 2: %i valid of %i\n", n, t);
	
	return 0;

}
