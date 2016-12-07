#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct TargetVal {
	char	Name[64];
	int	Value;
};

struct TargetVal Values[]={
	{"children", 3},
	{"cats", 7},
	{"samoyeds", 2},
	{"pomeranians", 3},
	{"akitas", 0},
	{"vizslas", 0},
	{"goldfish", 5},
	{"trees", 3},
	{"cars", 2},
	{"perfumes", 1},
	{"",0}
};

int main(int argc, char *argv[]) {
	char buffer[1024];
	int day2=0;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		char *parts[16];
		char *c;
		int p=1;
		int i, x, a, valid=1;

		memset(parts, 0, sizeof(parts));
		
		parts[0]=buffer;
		while((c=strchr(parts[p-1], ' '))) {
			if(*(c-1) == ':' || *(c-1) == ',') {
				*(c-1)=0;
			}
			*c=0;
			c++;
			parts[p++]=c;
		}

		a=atoi(parts[1]);
	
		for(day2=0;day2<2;day2++) {
			valid=1;
			
			for(i=2;valid && i<sizeof(parts)/sizeof(*parts) && parts[i];) {
				char *name=parts[i++];
				int val=atoi(parts[i++]);
	
				for(x=0;valid && Values[x].Name[0];x++) {
					if(!strcmp(Values[x].Name, name)) {
						if(day2 && (!strcmp(name, "cats") || !strcmp(name, "trees"))) {
							if(val <= Values[x].Value)
								valid=0;
						} else if(day2 && (!strcmp(name, "pomeranians") || !strcmp(name, "goldfish"))) {
							if(val >= Values[x].Value)
								valid=0;
						} else if(val != Values[x].Value) {
							valid=0;
						}
						//printf("Can't be Aunt %i because %s is %i, not %i\n", a, name, val, Values[x].Value);
					}
				}
			}
	
			if(valid) {
				printf("Valid aunt for day %i is %i\n", day2+1, a);
			}
		}
	}
		



	return 0;
}
