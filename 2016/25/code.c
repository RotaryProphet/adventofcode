#include <stdio.h>
#include <stdlib.h>

int run(int a, int b, int c, int d) {
	int Expected=0;
	int Count=0;
ln1:	d=a;
ln2:	c=7;
ln3:	b=362;
ln4:	d++;
ln5:	b--;
ln6:	if(b) goto ln4;
ln7:	c--;
ln8:	if(c) goto ln3;
ln9:	a=d;
ln10:	if(0) goto ln10;
ln11:	b=a;
ln12:	a=0;
ln13:	c=2;
ln14:	if(b) goto ln16;
ln15:	if(1) goto ln21;
ln16:	b--;
ln17:	c--;
ln18:	if(c) goto ln14;
ln19:	a++;
ln20:	if(1) goto ln13;
ln21:	b=2;
ln22:	if(c) goto ln24;
ln23:	if(1) goto ln27;
ln24:	b--;
ln25:	c--;
ln26:	if(1) goto ln22;
ln27:	if(0) goto ln27;
ln28:	if(b == Expected) {
		Expected=(Expected+1) & 1;
		if(++Count > 1000) {
			return 1;
		}
	} else {
		return 0;
	}
ln29:	if(a) goto ln10;
ln30:	if(1) goto ln9;
	return 0;
}

int main(int argc, char *argv[]) {
	int i;
	for(i=0;;i++) {
		if(run(i,0,0,0)) {
			printf("Day 1: %i\n", i);
			exit(0);
		}
	}
}
