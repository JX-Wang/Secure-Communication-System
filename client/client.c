#include <math.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "enc.h"
#define SERVER_PORT 8048
 
 
int main(){
 
    int clientSocket;
    struct sockaddr_in serverAddr;
    char sendbuf[200];
    char recvbuf[200];
    int iDataNum;
	char p[10], g[10];
	char B, yb[10];
	char key[4];
    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

	perror("socket");
	return 1;}

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
	perror("connect");
	return 1;}
    printf("连接到主机...\n");

    // diffie-hellman
	printf("Diffie-Hellman\n");
	printf("waitting for p from server\n");
	
	// recv p from server
	iDataNum = recv(clientSocket, recvbuf, 200, 0);
	printf("p is %s\n", recvbuf);
	strncpy(p, recvbuf, strlen(recvbuf));
	memset(recvbuf, '\0', sizeof(recvbuf));

	// send g to server
	printf("input g:%s", sendbuf);
	gets(sendbuf);
	send(clientSocket, sendbuf, strlen(sendbuf), 0);
	strncpy(g, sendbuf, strlen(g));
	
	printf("p is %s, g is %s\n", p, g);

	// recv yb
	printf("waiting for yb from server!\n");
	recv(clientSocket, recvbuf, 200, 0);
    strncpy(yb, recvbuf, strlen(recvbuf));
	printf("recv from server yb is:%s\n", yb);
	memset(sendbuf, '\0', sizeof(sendbuf));
    memset(recvbuf, '\0', sizeof(recvbuf));

	// random B
	double t_b;
	printf("input B:");
	scanf("%lf", &t_b);
	printf("\n");

	// ya and send ya
	double ya;
	ya = fmod(pow(atof(g), t_b), atof(p));
	printf("ya is:%lf\n", ya);  // check
	char t_ya[10];
	sprintf(t_ya, "%.0lf", ya);  // double -> char
	strncpy(sendbuf, t_ya, strlen(t_ya));
	send(clientSocket, sendbuf, strlen(sendbuf), 0);
	printf("ya is :%s\n", ya);

	double t_key;
   	//t_key = atof(t_key);
	t_key = fmod(pow(atof(yb), t_b), atof(p));
	printf("final key is: %.0lf\n", t_key);

	sprintf(key, "%.0lf", t_key);  
	/* generate encryption key from user entered key */
	if(!PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key),NULL,0,1000,32,key)){
		printf("Error in key generation\n");
		exit(1);
	}
	unsigned char aad[16]="abcdefghijklmnop";
	/*
	unsigned char iv[16] = {'a', 'b', 'c', 'd', 
							'e', 'f', 'g', 'h', 
							'i', 'j', 'k', 'a', 
							'b', 'c', 'd', 'e'
						};
	unsigned char tag[32] = {'a', 'b', 'c', 'd', 
							 'e', 'f', 'g', 'h', 
							 'i', 'j', 'k', 'a', 
							 'b', 'c', 'd', 'e',
							 'a', 'b', 'c', 'd', 
							 'e', 'f', 'g', 'h', 
							 'i', 'j', 'k', 'a', 
							 'b', 'c', 'd', 'e'
						};
 	*/
    while(1){
		unsigned char plaintext[1024];
		unsigned char ciphertext[1024+EVP_MAX_BLOCK_LENGTH];
		unsigned char tag[16];
		int k;
		printf("发送消息:");
		scanf("%s", plaintext);
		printf("\n");
		/* generate random IV */
		unsigned char iv[32];
		while(!RAND_bytes(iv,sizeof(iv)));
		
		/* encrypt the text and print on STDOUT */
		k = encrypt(plaintext, strlen(plaintext), aad, sizeof(aad), key, iv, ciphertext, tag);
		printf("iv is %s\n", iv);
		printf("sizeof, strlen:%d %d\n", sizeof(iv), strlen(iv));
		printf("tag is %s\n", tag);
		printf("sizeof, strlen:%d %d\n", sizeof(tag), strlen(tag));
		printf("send clipher text is:%s\n", ciphertext);
		printf("sizeof, strlen:%d %d\n", sizeof(ciphertext), strlen(ciphertext));
		memcpy(sendbuf, iv, 32);
		memcpy(sendbuf+32, tag, 16);
		memcpy(sendbuf+48, ciphertext, strlen(ciphertext));
		printf("send buffer is %s\n", sendbuf);
		printf("send buffer len is %d\n", strlen(sendbuf));
		send(clientSocket, sendbuf, strlen(sendbuf), 0);
		memset(sendbuf,'\0', sizeof(sendbuf));
		if(strcmp(plaintext, "quit") == 0)break;
		
		printf("读取消息:");
		recvbuf[0] = '\0';
		iDataNum = recv(clientSocket, recvbuf, 200, 0);
		recvbuf[iDataNum] = '\0';
		printf("%s\n", recvbuf);
	}
    
	close(clientSocket);
    return 0;
}

