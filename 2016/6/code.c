#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 8
int main(int argc, char *argv[]) {
	char buf[1024];
	char l[LEN][26];

	memset(l, 0, sizeof(l));

	while(fgets(buf, sizeof(buf), stdin)) {
		char *c;
		if((c=strchr(buf, '\n'))) {
			*c=0;
		}
		if((c=strchr(buf, '\r'))) {
			*c=0;
		}
		
		for(int i=0;buf[i] && i<LEN;i++) {
			int a = buf[i]-'a';
			l[i][a]++;
		}	
	}

	printf("Day 1: '");
	for(int i=0;i<LEN;i++) {
		int h=0;
		int hj=0;
		for(int j=0;j<26;j++) {
			if(l[i][j] > h) {
				h=l[i][j];
				hj=j;
			}
		}
		printf("%c", hj+'a');
	}
	printf("'\nDay 2: '");
	for(int i=0;i<LEN;i++) {
		int h=0;
		int hj=0;
		for(int j=0;j<26;j++) {
			if(l[i][j] < h || (l[i][j] && !h)) {
				h=l[i][j];
				hj=j;
			}
		}
		printf("%c", hj+'a');
	}
	printf("'\n");
	return 0;

}
