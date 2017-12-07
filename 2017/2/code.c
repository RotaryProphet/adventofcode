#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	char Buffer[4096];
	unsigned int Checksum=0, Checksum2=0;;
	int Row;
	while(fgets(Buffer, sizeof(Buffer), stdin)) {
		unsigned int Smallest=1<<31, Largest=0;
		unsigned int Nums[1024];
		int i, j;
		char *p;
		p=Buffer;
		int pos=0;

		memset(Nums, 0, sizeof(Nums));
		do {
			int num = atoi(p);
			Nums[pos++]=num;
			if(num > Largest)
				Largest=num;
			if(num < Smallest)
				Smallest=num;

			while(*p && !isspace(*p)) {
				p++;
			}
			while(*p && isspace(*p)) {
				p++;
			}
		} while(*p);

		for(i=0;i<pos;i++) {
			for(j=0;j<pos;j++) {
				if(i == j || Nums[i] < Nums[j]) {
					continue;
				}
				if(Nums[i] % Nums[j] == 0) {
					//printf("Row %i dividing %u by %u\n", Row, Nums[i], Nums[j]);
					Checksum2 += Nums[i] / Nums[j];
				}
			}
		}
		Checksum += Largest - Smallest;
		Row++;
	}
	printf("Day 1 checksum is %u\n", Checksum);
	printf("Day 2 checksum is %u\n", Checksum2);
	
	return 0;

}
