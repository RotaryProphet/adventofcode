#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char Buffer[4096];
	char c;
	int i;
	int totalnum=0;
	unsigned int total=0;

	memset(Buffer, 0, sizeof(Buffer));

	for(i=0;(c = getchar()) >= '0';i++) {
		Buffer[i]=c-'0';
		Buffer[i+1] = Buffer[0];
		totalnum++;
	}
	printf("Got %i characters...\n", i);

	for(i=0;Buffer[i+1]>0;i++) {
		if(Buffer[i] == Buffer[i+1]) {
			total+=Buffer[i];
		}
	}

	printf("Day 1 Total is %i\n", total);

	total=0;
	for(i=0;i<totalnum/2;i++) {
		int next = i+(totalnum/2);
		if(Buffer[i] == Buffer[next]) {
			total+=Buffer[i];
		}
	}
	printf("Day 2 Total is %i\n", total*2);
	return 0;
}
