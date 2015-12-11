#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char buffer[4096];
	int unpacked=0;
	int packed=0;
	int newstr=0;
	int line=0;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		int p=0;
		int u=1;
		int n=6;

		line++;

		if(buffer[0] != '"') {
			printf("Error on line %i: No starting quote\n", line);
			exit(1);
		}

		for(u=1;buffer[u];u++) {
			switch(buffer[u]) {
			case '\n':
				printf("Error on line %i: No ending quote\n", line);
				exit(1);
			case '"':
				unpacked += ++u;
				packed += p;
				newstr += n;
				goto DONELOOP;
			case '\\':
				if(buffer[u+1] == 'x') {
					u+=3;
					n+=5;
				} else {
					u++;
					n+=4;
				}
				p++;
				break;
			default:
				p++;
				n++;
				break;
			}
		}
		printf("Error on line %i: No ending nl or quote\n", line);
		exit(1);
DONELOOP:
		printf("Line %i has %i unpacked %i packed %i new\n", line, u, p, n);
		printf(" \b");
	}

	printf("\nNeed total of %i unpacked bytes, %i packed bytes\n", unpacked, packed);
	printf("Need total of %i for newstr\n", newstr);
	printf("Packing saves %i bytes\n", unpacked - packed);
	printf("Need %i more for newstr\n", newstr-unpacked);

	return 0;
}
