#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lights[1000][1000]={{0}};
int lights2[1000][1000]={{0}};

int main(int argc, char *argv[]) {
	char buffer[128], *p;
	int cmd;
	char *x1p, *x2p, *y1p, *y2p;
	int x1, x2, y1, y2, x, y;
	int line=0;
	int total1=0, total2=0;

	while(fgets(buffer, sizeof(buffer), stdin)) {
		line++;
		if((p=strchr(buffer, '\n'))) {
			*p=0;
		}

		if(!strncmp(buffer, "turn on", 7)) {
			cmd=1;
			p=buffer+8;
		} else if(!strncmp(buffer, "turn off", 8)) {
			cmd=2;
			p=buffer+9;
		} else if(!strncmp(buffer, "toggle", 6)) {
			cmd=3;
			p=buffer+7;
		} else {
			printf("Error on line %i: Invalid command '%s'\n", line, buffer);
			exit(1);
		}

		//printf("%s ", buffer);
		x1p=p;
		p=strchr(p, ',');
		*p=0;
		y1p=p+1;
		p=strchr(y1p, ' ');
		*p=0;
		x2p=strchr(p+1, ' ');
		p=strchr(x2p, ',');
		*p=0;
		y2p=p+1;
		x1=atoi(x1p);
		y1=atoi(y1p);
		x2=atoi(x2p);
		y2=atoi(y2p);

		//printf("%i (%i,%i)-(%i,%i)\n", cmd, x1, y1, x2, y2);

		for(x=x1;x<=x2;x++) {
			for(y=y1;y<=y2;y++) {
				switch(cmd) {
				case 1: // turn on
					lights[x][y]=1;
					lights2[x][y]++;
					break;
				case 2: // turn off
					lights[x][y]=0;
					lights2[x][y] = (lights2[x][y] ? lights2[x][y]-1 : 0);
					break;
				case 3: // Toggle
					lights[x][y] = (lights[x][y] ? 0 : 1);
					lights2[x][y] += 2;
					break;
				}
			}
		}
	}

	for(x=0;x<1000;x++) {
		for(y=0;y<1000;y++) {
			total1+=lights[x][y];
			total2+=lights2[x][y];
		}
	}
	printf("Day 1: Total of %i lights on\n", total1);
	printf("Day 2: Total of %i brightness\n", total2);

	return 0;
}
