#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <openssl/aes.h>

char *HexCode(unsigned char* data, int len){
	//printf("2222222222222222\n");
	char *rst;
	rst = (char *)malloc(260);
    int i = 0;
    for(; i < len; i++){
    	printf("%02x", (unsigned int)data[i]);
		sprintf(&rst[i*2], "%02x", (unsigned int)data[i]);
		//strcpy(rst, data[i]);
	}
    printf("\n");
	printf("rst is:%s\n", rst);
	return rst;
}


char aes_decrypt(char* userkey, unsigned char* data){
    const int len = 3;
    //char userkey[AES_BLOCK_SIZE];
    //unsigned char *data = malloc(AES_BLOCK_SIZE*len);
    unsigned char *cipher = malloc(AES_BLOCK_SIZE*len);
    unsigned char *plain = malloc(AES_BLOCK_SIZE*len);

    int i;
    AES_KEY key;

    //memset((void*)userkey, 0, AES_BLOCK_SIZE);
    //memset((void*)data, 0, AES_BLOCK_SIZE*len);
    memset((void*)cipher, 0, AES_BLOCK_SIZE*len);
    memset((void*)plain, 0, AES_BLOCK_SIZE*len);

    //strcpy(userkey, "userkey");
    //strcpy(data, "original text");
    printf("original text:\n");
    //HexCode(data, AES_BLOCK_SIZE*len);

    //AES_set_encrypt_key(userkey, 256, &key);
    //for(i = 0; i < len; i++)
    //AES_ecb_encrypt(data+i*AES_BLOCK_SIZE, cipher+i*AES_BLOCK_SIZE, &key, AES_ENCRYPT);
    //printf("cipher:%s\n", cipher);
	//char enc_rst[260];
    //strncpy(enc_rst, HexCode(cipher, AES_BLOCK_SIZE*len), 256);

    AES_set_decrypt_key(userkey, 256, &key);
    for(i = 0; i < len; i++)
    AES_ecb_encrypt(cipher+i*AES_BLOCK_SIZE, plain+i*AES_BLOCK_SIZE, &key, AES_DECRYPT);
    printf("plain:%s\n", plain);
	char dec_rst[260];
    //strncpy(dec_rst, HexCode(plain, AES_BLOCK_SIZE*len), 256);
	strncpy(dec_rst, plain, strlen(plain));
    //free(data);
    //free(cipher);
    //free(plain);
    return dec_rst;
}

char aes_encrypt(char* userkey, unsigned char* data){
	const int len = 3;
    //char userkey[AES_BLOCK_SIZE];
    //unsigned char *data = malloc(AES_BLOCK_SIZE*len);
    unsigned char *cipher = malloc(AES_BLOCK_SIZE*len);
    unsigned char *plain = malloc(AES_BLOCK_SIZE*len);

    int i;
    AES_KEY key;

    //memset((void*)userkey, 0, AES_BLOCK_SIZE);
    //memset((void*)data, 0, AES_BLOCK_SIZE*len);
    memset((void*)cipher, 0, AES_BLOCK_SIZE*len);
    memset((void*)plain, 0, AES_BLOCK_SIZE*len);

    //strcpy(userkey, "userkey");
    //strcpy(data, "original text");
    printf("original text:\n");
    //HexCode(data, AES_BLOCK_SIZE*len);

    AES_set_encrypt_key(userkey, 256, &key);
    for(i = 0; i < len; i++)
    AES_ecb_encrypt(data+i*AES_BLOCK_SIZE, cipher+i*AES_BLOCK_SIZE, &key, AES_ENCRYPT);
    printf("cipher:%s\n", cipher);
	//sprintf(cipher, "%02x", cipher);
    //printf("cipher:%02x\n", cipher);
    char *enc_rst;
	//printf("cpiher size:%d\n", strlen(cipher));
    enc_rst = HexCode(cipher, AES_BLOCK_SIZE*len);
	printf("1111111111111111111\n");
	printf("cipher is:%s\n", enc_rst);
	//strcpy(enc_rst, cipher);
	//printf("1");
    //free(data);
    //free(cipher);
    //free(plain);

    return enc_rst;
}

//int main(){
//	aes_decrypt("10", "nihao");
//}
