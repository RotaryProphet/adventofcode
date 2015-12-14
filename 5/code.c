#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t strnlen(const char *c, size_t len);

char invalids[][3]={ "ab", "cd", "pq", "xy" ,"" };

int main(int argc, char *argv[]) {
	int num[2]={0,0};
	char buffer[128];

	while(fgets(buffer, sizeof(buffer), stdin)) {
		int l = strnlen(buffer, sizeof(buffer));
		int i, x;
		int vowels=0;
		int doubles=0;
		int forbidden=0;

		char lb[3]={0, 0, 0};
		int repeated=0;
		int between=0;

		for(i=0;i<l;i++) {
			if(buffer[i] == 'a' || buffer[i] == 'e' || buffer[i] == 'i' || buffer[i] == 'o' || buffer[i] == 'u') {
				vowels++;
			}
			if(buffer[i] == buffer[i+1]) {
				doubles++;
			}
			for(x=0;invalids[x][0];x++) {
				if(!strncmp(&buffer[i], invalids[x], 2)) {
					forbidden=1;
					break;
				}
			}

			if(i+3 < l) {
				memcpy(lb, &buffer[i], 2);
				if(strstr(&buffer[i+2], lb)) {
					repeated=1;
				}
			}
			
			if(i+2 < l && buffer[i] == buffer[i+2]) {
				between++;
			}
		}
		if(vowels >= 3 && doubles > 0 && forbidden == 0) {
			num[0]++;
		}
		if(repeated > 0 && between > 0) {
			num[1]++;
		}
	}

	printf("Day 1: %i nice words\n", num[0]);
	printf("Day 2: %i nice words\n", num[1]);

	return 0;
}
