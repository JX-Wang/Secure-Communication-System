#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

void HexCode(unsigned char* data, int len){
	int i = 0;
	for(; i < len; i++)
	printf("%02x", (unsigned int)data[i]);
	printf("\n");
}


int main(){
	const int len = 3;
	char userkey[AES_BLOCK_SIZE];
	unsigned char *data = malloc(AES_BLOCK_SIZE*len);
	unsigned char *cipher = malloc(AES_BLOCK_SIZE*len);	
	unsigned char *plain = malloc(AES_BLOCK_SIZE*len);
	
	int i;
	AES_KEY key;
	
	memset((void*)userkey, 0, AES_BLOCK_SIZE);
	memset((void*)data, 0, AES_BLOCK_SIZE*len);
	memset((void*)cipher, 0, AES_BLOCK_SIZE*len);
	memset((void*)plain, 0, AES_BLOCK_SIZE*len);

	strcpy(userkey, "userkey");
	strcpy(data, "original text");
	printf("original text:\n");
	HexCode(data, AES_BLOCK_SIZE*len);

	AES_set_encrypt_key(userkey, 128, &key);
	for(i = 0; i < len; i++)
	AES_ecb_encrypt(data+i*AES_BLOCK_SIZE, cipher+i*AES_BLOCK_SIZE, &key, AES_ENCRYPT);
	printf("cipher:\n");
	HexCode(cipher, AES_BLOCK_SIZE*len);

	AES_set_decrypt_key(userkey, 128, &key);
	for(i = 0; i < len; i++)
	AES_ecb_encrypt(cipher+i*AES_BLOCK_SIZE, plain+i*AES_BLOCK_SIZE, &key, AES_DECRYPT);
	printf("plain:\n");
	HexCode(plain, AES_BLOCK_SIZE*len);

	free(data);
	free(cipher);
	free(plain);

	return 0;
}
