#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

const char input[]="uqwqemis";

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
	char pass[9];
	char pass2[9];
	int passl=0;
	int pass2l=0;
	unsigned int i;

	memset(pass, 0, sizeof(pass));
	memset(pass2, 0, sizeof(pass2));

	for(i=0;i < (unsigned int)-1;i++) {
		snprintf(inbuf, sizeof(inbuf), "%s%u", input, i);
		str2md5(inbuf, strlen(inbuf), outbuf, sizeof(outbuf));	
		if(!strncmp(outbuf, "00000", 5)) {
			printf("'%s' gets '%s'\n", inbuf, outbuf);
			if(passl < 8) {
				printf("Found digit %i = '%c'\n", passl, outbuf[5]);
				printf("'%s' gets '%s'\n", inbuf, outbuf);
				pass[passl++]=outbuf[5];
				if(passl == 8) {
					printf("Day 1 pass is '%s'\n", pass);
				}
			}
			if(pass2l < 8) {
				int p=outbuf[5]-'0';
				if(p >= 0 && p <= 7 && pass2[p] == 0) {
					pass2[p]=outbuf[6];
					pass2l++;
					if(pass2l==8) {
						printf("Day 2 pass is '%s'\n", pass2);
					}
				}
			}
		}

		if(passl == 8 && pass2l == 8) {
			break;
		}
	}

	return 0;
}
