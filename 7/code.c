#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char *p;

	if((p=strrchr(argv[0], '/'))) {
		*p=0;
		chdir(argv[0]);
	}

	return system("./code.pl");
}
