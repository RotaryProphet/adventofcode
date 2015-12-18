#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define NumX 100
#define NumY 100

uint8_t Initial[NumX][NumY];
uint8_t Lights[NumX][NumY];
uint8_t Working[NumX][NumY];

#define MIN(a,b) (a > b ? b : a)
#define MAX(a,b) (a > b ? a : b)

int main(int argc, char *argv[]) {
	char buffer[1024];
	int x, y=0;
	int i, lightson;

	memset(Initial, 0, sizeof(Initial));
	while(y < NumY && fgets(buffer, sizeof(buffer), stdin)) {
		for(x=0;x<NumX && buffer[x];x++) {
			if(buffer[x] == '#') {
				Initial[x][y]=1;
			}
		}
		y++;
	}

	memcpy(Lights, Initial, sizeof(Lights));
	for(i=0;i<100;i++) {
		lightson=0;
		memset(Working, 0, sizeof(Working));
		for(x=0;x<NumX;x++) {
			for(y=0;y<NumY;y++) {
				int nx, ny,on=0;
				for(nx=MAX(x-1,0);nx<=MIN(x+1,NumX-1);nx++) {
					for(ny=MAX(y-1,0);ny<=MIN(y+1,NumY-1);ny++) {
						if((nx!=x || ny!=y) && Lights[nx][ny]) {
							on++;
						}
					}
				}
				if((Lights[x][y] && on == 2) || on == 3) {
					Working[x][y]=1;
					lightson++;
				}
			}
		}
		memcpy(Lights, Working, sizeof(Lights));
	}

	printf("Day 1: %i lights on\n", lightson);

	memcpy(Lights, Initial, sizeof(Lights));
	Lights[0][0]=1;
	Lights[0][NumY-1]=1;
	Lights[NumX-1][0]=1;
	Lights[NumX-1][NumY-1]=1;
	for(i=0;i<100;i++) {
		lightson=0;
		memset(Working, 0, sizeof(Working));
		for(x=0;x<NumX;x++) {
			for(y=0;y<NumY;y++) {
				int nx, ny,on=0;
				if((x==0 || x==NumX-1) && (y==0 || y==NumY-1)) {
					Working[x][y]=1;
					lightson++;
					continue;
				}
				for(nx=MAX(x-1,0);nx<=MIN(x+1,NumX-1);nx++) {
					for(ny=MAX(y-1,0);ny<=MIN(y+1,NumY-1);ny++) {
						if((nx!=x || ny!=y) && Lights[nx][ny]) {
							on++;
						}
					}
				}
				if((Lights[x][y] && on == 2) || on == 3) {
					Working[x][y]=1;
					lightson++;
				}

			}
		}
		memcpy(Lights, Working, sizeof(Lights));
	}

	printf("Day 2: %i lights on\n", lightson);
	return 0;
}
