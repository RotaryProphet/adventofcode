#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

//const char input[]="cuanljph";
const char input[]="cuanljph";

char keys[64][33];

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

int checktrip(const char *str) {
	size_t len = strlen(str);
	for(size_t i=0;i<len-2;i++) {
		if(str[i] == str[i+1] && str[i] == str[i+2]) {
			return str[i];
		}
	}
	return 0;
}

void dohash(const char *inbuf, size_t length, char *dest, size_t dest_len, int extra_hashes) {
	char outbuf[33], tmpbuf[33];
	str2md5(inbuf, length, outbuf, sizeof(outbuf));

	for(int i=0;i<extra_hashes;i++) {
		memcpy(tmpbuf, outbuf, sizeof(outbuf));
		str2md5(tmpbuf, strlen(tmpbuf), outbuf, sizeof(outbuf));
	}

	strncpy(dest, outbuf, dest_len);
}

int process(int extrahash) {
	char inbuf[256];
	char hashes[1001][33];
	int key=0;
	int i;

	// Seed
	for(i=0; i<1001; i++) {
		snprintf(inbuf, sizeof(inbuf), "%s%i", input, i);
		dohash(inbuf, strlen(inbuf), hashes[i], sizeof(*hashes), extrahash);
	}
	for(i=0;; i++) {
		int c;
		char teststr[6];

		if((c=checktrip(hashes[0]))) {
			memset(teststr, c, 5);
			teststr[5]=0;
			for(int j=1;j<1001;j++) {
				if(strstr(hashes[j], teststr)) {
					if(++key == 64) {
						return i;
					}
					break;
				}
			}
		}
		memmove(&hashes[0], &hashes[1], sizeof(hashes)-sizeof(*hashes));
		snprintf(inbuf, sizeof(inbuf), "%s%i", input, i+1001);
		dohash(inbuf, strlen(inbuf), hashes[1000], sizeof(*hashes), extrahash);
	}
}

int main(int argc, char *argv[]) {
	printf("Day 1 key 64 is index %i\n", process(0));
	printf("Day 2 key 64 is index %i\n", process(2016));
	
	return 0;
}
