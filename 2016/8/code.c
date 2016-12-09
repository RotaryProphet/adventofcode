#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 50
#define HEIGHT 6
#define MIN(a,b) (a > b ? b : a)

char screen[WIDTH][HEIGHT];

int display() {
	int c=0;
	printf(" |");
	for(int x=0;x<WIDTH;x++) {
		printf("%i", x/10);
	}
	printf("\n |");
	for(int x=0;x<WIDTH;x++) {
		printf("%i", x % 10);
	}
	puts("");
	for(int y=0;y<HEIGHT;y++) {
		printf("%i|", y);
		for(int x=0;x<WIDTH;x++) {
			if(screen[x][y]) {
				c++;
				printf("#");
			} else {
				printf(" ");
			}
		}
		puts("");
	}
	return c;
}

void do_rect(int w, int h) {
	for(int y=0;y<MIN(h,HEIGHT);y++) {
		for(int x=0;x<MIN(w,WIDTH);x++) {
			screen[x][y]=1;
		}
	}
}

void rot_c(int x, int c) {
	char new[HEIGHT];
	memset(new, 0, sizeof(new));

	for(int y=0;y<HEIGHT;y++) {
		int newy = (y+c)%HEIGHT;
		new[newy] = screen[x][y];
	}
	for(int y=0;y<HEIGHT;y++) {
		screen[x][y] = new[y];
	}
}

void rot_r(int y, int c) {
	char new[WIDTH];
	memset(new, 0, sizeof(new));
	for(int x=0;x<WIDTH;x++) {
		int newx = (x+c)%WIDTH;
		new[newx] = screen[x][y];
	}
	for(int x=0;x<WIDTH;x++) {
		screen[x][y]=new[x];
	}
}

int main(int argc, char *argv[]) {
	char buf[1024]="";
	int a, b;
	int count;

	memset(screen, 0, sizeof(screen));

	while(fgets(buf, sizeof(buf), stdin)) {
		char *c;
		if((c=strchr(buf, '\n'))) {
			*c=0;
		}
		if((c=strchr(buf, '\r'))) {
			*c=0;
		}

		puts(buf);
		if(!strncmp(buf, "rect", 4)) {
			c=strchr(buf, ' ')+1;
			a=atoi(c);
			c=strchr(buf, 'x')+1;
			b=atoi(c);
			do_rect(a, b);
		} else if(!strncmp(buf, "rotate row", strlen("rotate row"))) {
			c=strchr(buf, '=')+1;
			a=atoi(c);
			c=strchr(c, ' ')+4;
			b=atoi(c);
			rot_r(a, b);
		} else if(!strncmp(buf, "rotate column", strlen("rotate column"))) {
			c=strchr(buf, '=')+1;
			a=atoi(c);
			c=strchr(c, ' ')+4;
			b=atoi(c);
			rot_c(a, b);
		} else {
			printf("Unknown command '%s'\n", buf);
			exit(0);
		}
		count=display();
	}
	printf("Pixels = %i\n", count);
	return 0;
}
