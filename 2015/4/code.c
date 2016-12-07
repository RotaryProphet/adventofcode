#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

const char input[]="yzbqklnj";

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

int main(int argc, char *argv[]) {
	char inbuf[256];
	char outbuf[33];
	unsigned int i;
	int found1=0;
	int found2=0;

	for(i=0;i < (unsigned int)-1;i++) {
		snprintf(inbuf, sizeof(inbuf), "%s%u", input, i);
		str2md5(inbuf, strlen(inbuf), outbuf, sizeof(outbuf));	
		if(!found1 && !strncmp(outbuf, "00000", 5)) {
			printf("Day1: Input '%s' yields md5 '%s': %u\n", inbuf, outbuf, i);
			found1=1;
		}
		if(!found2 && !strncmp(outbuf, "000000", 6)) {
			printf("Day2: Input '%s' yields md5 '%s': %u\n", inbuf, outbuf, i);
			found2=1;
		}
		if(found1 && found2) {
			break;
		}
	}

	return 0;
}
