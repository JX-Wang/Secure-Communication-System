#include <stdio.h>
#include "decrypt.h"

int main(){
	char key[] = "10";
	char plain[] = "nihaowoshiwangjunxionglaizihitwh";
	printf("key:%s\nplain:%s\n", key, plain);

	char* cipher;
	cipher = aes_encrypt(key, plain);
	printf("cipher is %s\n", cipher);

	char* rst_plain;
	rst_plain = aes_decrypt(key, cipher);
	printf("After decrypt plain is: %s\n", rst_plain);

}
