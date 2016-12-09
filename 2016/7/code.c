#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SupportABBA(char *s) {
	int b=0;
	int r=0;
	int i;

	for(i=0;s[i];i++) {
		if(s[i] == '[') {
			b++;
		} else if(s[i] == ']') {
			b--;
		} else if(s[i] != s[i+1] && s[i] == s[i+3] && s[i+1] == s[i+2]) {
			if(b) {
				return 0;
			} else {
				r=1;
			}
		}
	}
	return r;
}

int CheckBracketsFor(char *buf, char *sub) {
	char *c;
	for(c=strchr(buf, '['); c && *c; c++) {
		if(*c == ']' && (c=strchr(c, '['))==NULL) {
			break;
		} else if(!strncmp(c, sub, 3)) {
			return 1;
		}
	}
	return 0;
}

int SupportSSL(char *s) {
	char *c;
	char sub[4]="";
	for(c=s; c && *c; c++) {
		if(*c == '[' && (c=strchr(c, ']'))==NULL) {
			break;
		} else if(c[0] == c[2] && c[0] != c[1] && c[1] != '[') {
			sub[0] = c[1];
			sub[1] = c[0];
			sub[2] = c[1];
			sub[3] = 0;
			if(CheckBracketsFor(s, sub)) {
				return 1;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char buf[1024]="";
	int r=0;
	int r2=0;

	while(fgets(buf, sizeof(buf), stdin)) {
		char *c;
		if((c=strchr(buf, '\n'))) {
			*c=0;
		}
		if((c=strchr(buf, '\r'))) {
			*c=0;
		}
		r+=SupportABBA(buf);
		r2+=SupportSSL(buf);
		memset(buf, 0, sizeof(buf));
	}
	printf("%i support ABBA\n", r);
	printf("%i support SSL\n", r2);

	return 0;

}
