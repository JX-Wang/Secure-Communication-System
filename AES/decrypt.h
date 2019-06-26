#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <openssl/aes.h>

char *HexCode(unsigned char* data, int len){
	char *rst;
	rst = (char *)malloc(260);
    int i = 0;
    for(; i < len; i++){
		sprintf(&rst[i*2], "%02x", (unsigned int)data[i]);
	}
    printf("\n");
	printf("rst is:%s\n", rst);
	return rst;
}


char *aes_decrypt(char* userkey, unsigned char* data){
    const int len = 3;
    unsigned char *cipher = malloc(AES_BLOCK_SIZE*len);
    unsigned char *plain = malloc(AES_BLOCK_SIZE*len);

    int i;
    AES_KEY key;

    memset((void*)cipher, 0, AES_BLOCK_SIZE*len);
    memset((void*)plain, 0, AES_BLOCK_SIZE*len);

    //printf("original text:\n");
	printf("from server cipher is:%s\n", data);
    AES_set_decrypt_key(userkey, 256, &key);
    for(i = 0; i < len; i++)
    AES_gcm_encrypt(cipher+i*AES_BLOCK_SIZE, plain+i*AES_BLOCK_SIZE, &key, AES_DECRYPT);
    printf("plain:%s\n", plain);
	char *dec_rst;
    dec_rst = HexCode(plain, AES_BLOCK_SIZE*len);
	printf("plain is: %s\n", dec_rst);
    return dec_rst;
}

char *aes_encrypt(char* userkey, unsigned char* data){
	const int len = 3;
    unsigned char *cipher = malloc(AES_BLOCK_SIZE*len);
    unsigned char *plain = malloc(AES_BLOCK_SIZE*len);

    int i;
    AES_KEY key;

    memset((void*)cipher, 0, AES_BLOCK_SIZE*len);
    memset((void*)plain, 0, AES_BLOCK_SIZE*len);

    printf("original text:\n");
    HexCode(data, AES_BLOCK_SIZE*len);

    AES_set_encrypt_key(userkey, 256, &key);
    for(i = 0; i < len; i++)
    AES_gcm_encrypt(data+i*AES_BLOCK_SIZE, cipher+i*AES_BLOCK_SIZE, &key, AES_ENCRYPT);
    printf("cipher:%s\n", cipher);
    char *enc_rst;
    enc_rst = HexCode(cipher, AES_BLOCK_SIZE*len);
	printf("1111111111111111111\n");
	printf("cipher is:%s\n", enc_rst);

    return enc_rst;
}
