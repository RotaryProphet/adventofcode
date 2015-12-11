#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLUSONE(l) (l > 'z' ? 'a' : l+1)

const char StartStr[]="cqjxjnds";
int NumPasswords=2;

void incstr(char *str) {
	size_t l;

	for(l=7;l>=0;l++) {
		str[l]=PLUSONE(str[l]);
		if(str[l] != 'a') {
			return;
		}
	}
}

int validate(char *str) {
	size_t l;
	int d=0;

	for(l=0;l<8;l++) {
		if(str[l] == 'i' || str[l] == 'o' || str[l] == 'l') {
			return 0;
		}
	}

	for(l=0;l<7;l++) {
		if(str[l] == str[l+1]) {
			l++;
			d++;
		}
	}

	if(d<2) {
		return 0;
	}

	for(l=0;l<6;l++) {
		if(	str[l+1] == PLUSONE(str[l])
		    &&	str[l+2] == PLUSONE(str[l+1])
		) {
			return 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char buffer[9];
	int found=0;

	strcpy(buffer, StartStr);
	for(;;) {
		incstr(buffer);
		if(validate(buffer)) {
			printf("Valid #%i: %s\n", ++found, buffer);
			if(found == NumPasswords) {
				break;
			}
		}
	}
	return 0;
}
