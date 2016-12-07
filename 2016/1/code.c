#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pos {
	int x;
	int y;
};

#define MAX(a,b) (a>b ? a : b)

void checkloc(int x, int y) {
	static struct pos hist[1024]={{0,0}};
	static int p=0;
	static int found=0;
	int i;

	if(found)
		return;
	
	hist[p].x=x;
	hist[p].y=y;
	for(i=0;i<p;i++) {
		if(x == hist[i].x && y == hist[i].y) {
			printf("Found twice, %i, %i, distance=%i\n", x, y, MAX(x, x*-1) + MAX(y, y*-1));
			found=1;
			break;
		}	
	}
	p++;
}

int main(int argc, char *argv[]) {
	char str[1024]={0};
	int d=0;
	int x=0;
	int y=0;
	char *c;
	int i;


	fgets(str, sizeof(str), stdin);
	for(c=str; c && *c;) {
		if(*c == 'R') {
			d++;
			//printf("Turn right");
		} else if(*c == 'L') {
			d--;
			//printf("Turn left");
		} else {
			printf("Error, unexpected character '%c'\n", *c);
			exit(1);
		}

		c++;
		switch(d) {
		case 4:
			d=0;
		case 0: // North
			for(i=y+1;i<=y+atoi(c);i++) {
				checkloc(x, i);
			}	
			y+=atoi(c);
			//printf("y+=%i\n", atoi(c));
			break;
		case 1: // East
			for(i=x+1;i<=x+atoi(c);i++) {
				checkloc(i, y);
			}
			x+=atoi(c);
			//printf("x+=%i\n", atoi(c));
			break;
		case 2: // South
			for(i=y-1;i>=y-atoi(c);i--) {
				checkloc(x, i);
			}
			y-=atoi(c);
			//printf("y-=%i\n", atoi(c));
			break;
		case -1:
			d=3;
		case 3:
			for(i=x-1;i>=x-atoi(c);i--) {
				checkloc(i, y);
			}
			x-=atoi(c);
			//printf("x-=%i\n", atoi(c));
			break;
		default:
			printf("Error, unexpected direction '%i'\n", d);
		}
		if((c=strchr(c, ' '))) {
			c++;
		}

	}
	printf("x distance is '%i', y distance is '%i'\n", x, y);

	if(x<0) {
		x*=-1;
	}
	if(y<0) {
		y*=-1;
	}
	printf("Total distance = %i\n", x+y);
	return 0;
}
