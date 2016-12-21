#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	char str[1024]={0};
	int64_t Start, End, i, Allowed=0;
	char *IPs=malloc((uint64_t)1<<32);
	int f=0;
	if(!IPs) {
		printf("Error allocating\n");
		return 1;
	}

	memset(IPs, 1, (uint64_t)1<<32);

	while(fgets(str, sizeof(str), stdin)) {
		printf("%s", str);
		char *c=strchr(str, '-');
		Start=atoll(str);
		if(c) {
			End=atoll(c+1);
		} else {
			End = Start;
		}
		for(i=Start;i<=End;i++) {
			IPs[i]=0;
		}
	}

	for(i=0;i<(uint64_t)1<<32;i++) {
		if(IPs[i]) {
			Allowed++;
			if(!f) {
				printf("Lowest IP is %lu\n", i);
				f=1;
			}
		}
	}
	printf("Total of %li allowed\n", Allowed);

	return 0;
}
