#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

const char Input[]="gdjjyniy";
const int TargetX = 3;
const int TargetY = 3;
int FoundSteps=0;

void str2md5(const char *str, size_t length, char *dest, size_t dest_len) {
	int n;
	MD5_CTX c;
	unsigned char digest[16];
	char out[33]={0};

	MD5_Init(&c);

	while (length > 0) {
		if (length > 512) {
			MD5_Update(&c, str, 512);
			length-=512;
			str+=512;
		} else {
			MD5_Update(&c, str, length);
			length=0;
		}
	}

	MD5_Final(digest, &c);
	for (n = 0; n < 16; ++n) {
		sprintf(&(out[n*2]), "%02x", (unsigned int)digest[n]);
	}
	strncpy(dest, out, dest_len);
}

void Process_R(const char *Steps, int x, int y, int Longest) {
	char *Buf, Hash[33], *InBuf;
	int idx=strlen(Steps);
	if(Longest == 0 && FoundSteps && idx >= FoundSteps) {
		return;
	}

	if(x == TargetX && y == TargetY) {
		if((Longest && FoundSteps < idx) || !Longest) {
			FoundSteps = idx;
			printf("Found (%i): %s\n", idx, Steps);
		}
		return;
	}

	Buf=malloc(idx+2);
	strcpy(Buf, Steps);
	Buf[idx+1]=0;

	InBuf=malloc(strlen(Input) + idx + 2);	
	strcpy(InBuf, Input);
	strcat(InBuf, Steps);
	str2md5(InBuf, strlen(InBuf), Hash, sizeof(Hash));
	free(InBuf);

	if(y < 3 && Hash[1] >= 'b' && Hash[1] <= 'f') {
		Buf[idx] = 'D';
		Process_R(Buf, x, y+1, Longest);
	}
	if(x < 3 && Hash[3] >= 'b' && Hash[3] <= 'f') {
		Buf[idx] = 'R';
		Process_R(Buf, x+1, y, Longest);
	}

	if(y > 0 && Hash[0] >= 'b' && Hash[0] <= 'f') {
		Buf[idx] = 'U';
		Process_R(Buf, x, y-1, Longest);
	}
	if(x > 0 && Hash[2] >= 'b' && Hash[2] <= 'f') {
		Buf[idx] = 'L';
		Process_R(Buf, x-1, y, Longest);
	}
	free(Buf);
}

int main(int argc, char *argv[]) {
	Process_R("", 0, 0, 0);
	puts("Day 2");
	FoundSteps = 0;
	Process_R("", 0, 0, 1);
	return 0;
}
