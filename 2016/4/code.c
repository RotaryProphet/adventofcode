#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getsum(char *str, char *sum) {
	int i, j, n;
	int hl;
	char l[27];

	memset(l, 0, sizeof(l));
	for(i=0;str[i];i++) {
		char a=str[i];
		if(a >= 'a' && a <= 'z') {
			l[ a-'a' ]++;
		}
	}

	for(i=0;i<5;i++) {
		hl=0;
		n=0;
		for(j=0;j<sizeof(l)/sizeof(*l);j++) {
			if(l[j] > n) {
				n=l[j];
				hl=j;
			}
		}
		l[hl]=0;
		sum[i]=hl+'a';
	}
}

void decode(char *in, char *out, int n) {
	int i;

	for(i=0;in[i];i++) {
		if(in[i] == '-') {
			out[i]=' ';
		} else {
			int a=in[i] - 'a';
			a+=n;
			a %= 26;
			out[i]=a+'a';
		}
	}
	out[i]=0;

}

int main(int argc, char *argv[]) {
	char buf[1024];
	int res=0;

	while(fgets(buf, sizeof(buf), stdin)) {
		char *c;
		char *sum=NULL;
		char newsum[6];
		char decoded[512];
		int zone=0;

		if((c=strchr(buf, ']'))) {
			*c=0;
		} else {
			continue;
		}

		if((c=strchr(buf, '['))) {
			*c=0;
			sum=c+1;
			while(*c != '-') {
				c--;
			}
			*c=0;
			zone=atoi(c+1);
		}

		memset(newsum, 0, sizeof(newsum));
		getsum(buf, newsum);
		//printf("'%s', %i, '%s' vs '%s'\n", buf, zone, sum, newsum);
		if(!strncmp(sum, newsum, sizeof(newsum))) {
			res+=zone;

			memset(decoded, 0, sizeof(decoded));
			decode(buf, decoded, zone);
			//printf("%i - %s\n", zone, decoded);
			if(!strncmp(decoded,"northpole object storage", sizeof(decoded))) {
				printf("Day 2: Zone %i = '%s'\n", zone, decoded);
			}
		}
			
	}
	printf("Day 1: Sum of zones = %i\n", res);
	return 0;

}
