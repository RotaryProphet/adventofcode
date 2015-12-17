#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char StartStr[9]="cqjxjnds";
int NumPasswords=2;

void incstr(char *str) {
	int l;

	for(l=7;l>=0;l--) {
		if(str[l]++ == 'z') {
			str[l] = 'a';
		} else {
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
		if(	str[l+1] == str[l]+1
		     && str[l+2] == str[l]+2
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
