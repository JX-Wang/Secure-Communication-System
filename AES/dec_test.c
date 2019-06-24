#include <stdio.h>
#include "decrypt.h"

int main(){
	char key[] = "10";
	char plain[] = "nihao";
	printf("key:%s\nplain:%s\n", key, plain);

	unsigned char cipher[256];
	strncpy(cipher, aes_encrypt(key, plain), strlen(cipher));
	printf("cipher is %s\n", cipher);

	unsigned char rst_plain[256];
	strncpy(rst_plain, aes_decrypt(key, cipher), 256);
	printf("After decrypt plain is: %s\n", rst_plain);

}
