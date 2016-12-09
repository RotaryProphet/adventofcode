#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

uint64_t Decomp_R(char *buf, size_t BufLen) {
	uint64_t r=0;

	for(char *c=buf; c < buf + BufLen; ) {
		if(*c == '(') {
			char *b=strchr(c, 'x')+1;
			int l1 = atoi(c+1);
			int l2 = atoi(b);
			c=strchr(c, ')')+1;

			r+=Decomp_R(c, l1) * l2;
			c+=l1;
		} else {
			r++;
			c++;
		}
	}
	return r;
}

int main(int argc, char *argv[]) {
	char buf[1024]="";
	char *comp=NULL;
	size_t complen=0;
	size_t comppos=0;

	char *uncomp=NULL;
	size_t uncomplen=0;
	size_t uncomppos=0;
	char *c;

	while(fgets(buf, sizeof(buf), stdin)) {
		for(c=buf;c && *c;c++) {
			if(isspace(*c)) {
				continue;
			}
			if(comppos == complen) {
				comp=realloc(comp, complen + 1024);
				complen+=1024;
			}
			comp[comppos++]=*c;
		}		
	}
	comp[comppos]=0;
	printf("Compressed length is %lu\n", comppos);

	c=comp;
	while(c && *c) {
		if(*c == '(') {
			char *b=strchr(c,'x')+1;
			int l1 = atoi(c+1);
			int l2 = atoi(b);
			c=strchr(c,')')+1;
			
			while(uncomppos + (l1*l2) >= uncomplen) {
				uncomp=realloc(uncomp, uncomplen+1024);
				uncomplen+=1024;
			}
			for(int i=0;i<l2;i++) {
				strncpy(&uncomp[uncomppos], c, l1);
				uncomppos+=l1;
			}
			c+=l1;
		} else {
			if(uncomppos == uncomplen) {
				uncomp=realloc(uncomp, uncomplen+1024);
				uncomplen+=1024;
			}
			uncomp[uncomppos++]=*c;
			c++;
		}
	}
	uncomp[uncomppos]=0;
	printf("Decompressed length is %lu\n", uncomppos);

	printf("Fully decompressed length is %lu\n", Decomp_R(comp, comppos));
	
	return 0;
}
