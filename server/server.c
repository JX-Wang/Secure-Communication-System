#include <math.h>
#include <sys/stat.h> 
#include <fcntl.h>
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
#include "dec.h"
#define SERVER_PORT 8048
/*
监听后，一直处于accept阻塞状态，
直到有客户端连接，
当客户端如数quit后，断开与客户端的连接
*/
 
int main()
{
 
	//调用socket函数返回的文件描述符
    int serverSocket;
 
	//声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char buffer[200];
    int iDataNum;
	char p[10], g[10];
    char A, ya[10], yb[10];
    char key[4];

	//socket函数，失败返回-1 
	//int socket(int domain, int type, int protocol);
	//第一个参数表示使用的地址类型，一般都是ipv4，AF_INET
	//第二个参数表示套接字类型：tcp：面向连接的稳定数据传输SOCK_STREAM
	//第三个参数设置为0
 
	if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("socket");
    	return 1;
	}
 
	bzero(&server_addr, sizeof(server_addr));

	//初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
 
	//ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

 
	//对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *) 
	//bind三个参数：服务器端的套接字的文件描述符，
	if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    	perror("connect");return 1;
	}
 
	//设置服务器上的socket为监听状态
	if(listen(serverSocket, 5) < 0) { 
    	perror("listen");
    	return 1;
	}

	while(1) { 
    	printf("监听端口: %d\n", SERVER_PORT);
 
	//调用accept函数后，会进入阻塞状态
	//accept返回一个套接字的文件描述符，这样服务器端便有两个套接字的文件描述符， 
	//serverSocket和client。
	//serverSocket仍然继续在监听状态，client则负责接收和发送数据
	//clientAddr是一个传出参数，accept返回时，传出客户端的地址和端口号
	//addr_len是一个传入-传出参数，传入的是调用者提供的缓冲区的clientAddr的长度，以避免缓冲区溢出。 
	//传出的是客户端地址结构体的实际长度。
	//出错返回-1
    client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
    if(client < 0){
		perror("accept");
		continue;
	}
 
	printf("等待消息...\n");
 
	//inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
 
	//表达式：char *inet_ntoa (struct in_addr);
 
	printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
	printf("Port is %d\n", htons(clientAddr.sin_port));
	
	//diffie-hellman
	/*
	//double p_bffer;
	scanf("input p %lf", p_buffer);
	send(client, p_buffer, sizeof(p_buffer), 0);
	//double g, g_buffer;
	iDataNum = recv(client, g_buffer, 1024, 0);
	
	if(iDataNum < 0){
	    perror("g_buffer recv error");
	    continue;}
	
 	//double A, Yb;
	scanf("input A %lf", yb);
	Yb = g_buffer^A%p_buffer;
	send(client, Yb, sizeof(yb), 0);
	
	//double ya;
	iDataNum = recv(client, ya, 1024, 0);

	if(iDataNum < 0){
            perror("ya recv error");
            continue;}

	*/
	printf("Diffie-Hellman\n");
	// send p to client
	printf("input p:");
	scanf("%s", &buffer);
	//printf("\n");
	printf("p is:%s\n", buffer);
	send(client, buffer, strlen(buffer), 0);
	strncpy(p, buffer, 10);
	printf("p is:%s\n", p);

	// recv g from client
	memset(buffer, '\0', sizeof(buffer));
	printf("waitting for g from client\n");
	recv(client, buffer, 200, 0);
	printf("g is:%s\n", buffer);
	strncpy(g, buffer, 10);

	printf("p is %s, g is %s\n", p, g);

	// random A
	double t_a;
    printf("input A:");
    scanf("%lf", &t_a);
    // printf("\n");

    // yb and send yb
    double yb;
    yb = fmod(pow(atof(g), t_a), atof(p));
	printf("yb is %.0lf\n", yb); // check
	char t_yb[10];
	sprintf(t_yb, "%.0lf", yb);
	printf("1\n");
    strncpy(buffer, t_yb, strlen(t_yb));
    send(client, buffer, strlen(buffer), 0);
	printf("2");
    printf("yb is :\n", yb);

    // recv ya from client
	memset(buffer, '\0', sizeof(buffer));
    recv(client, buffer, 200, 0);
    strncpy(ya, buffer, strlen(ya));
	printf("recv from client ya is:%s\n", ya);

    double t_key;
    //t_key = atof(t_key);
    t_key = fmod(pow(atof(ya), t_a), atof(p));
    printf("final key is: %.0lf\n", t_key);

	printf("step 0\n");
	sprintf(key, "%.0lf", t_key);
	printf("step 1\n");
	/* generate encryption key from user entered key */
	if(!PKCS5_PBKDF2_HMAC_SHA1(key, strlen(key),NULL,0,1000,32,key)){
		printf("Error in key generation\n");
		exit(1);
	}
	unsigned char aad[16] = "abcdefghijklmnop";
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
	while(1)
	    {
		printf("读取消息:");
		buffer[0] = '\0';
 		iDataNum = recv(client, buffer, 1024, 0);
		printf("origin cipher text is:%s\n", buffer);
		if(iDataNum < 0){
		    perror("recv null");
		    continue;}
		unsigned char pt[1024+EVP_MAX_BLOCK_LENGTH], ciphertext[1024];
		unsigned char iv[32], tag[16];
		printf("recv cipher lenght is:%d\n", strlen(buffer));
		printf("step 2\n");
		memcpy(iv, buffer, 32);
		printf("iv is:%s\n", iv);
		printf("sizeof, strlen:%d %d\n", sizeof(iv), strlen(iv));
		printf("step 3\n");
		memcpy(tag, buffer+32, 16); 
		printf("tag is:%s\n", tag);
		printf("sizeof, strlen:%d %d\n", sizeof(tag), strlen(tag));
		printf("step 4\n");
		memcpy(ciphertext, buffer+48, 14);
		printf("step 5\n");
		int k=8;
		k = decrypt(ciphertext, k, aad, sizeof(aad), tag, key, iv, pt);
		if(k>0){
			pt[k] = '\0';
			printf("after decrypt is:%s\n", pt);
		}
		else
			printf("Unreliable Decryption, maybe the encrypted data was tampered\n");
		buffer[iDataNum] = '\0';
 
		if(strcmp(pt, "quit") == 0)break;
		printf("%s\n", buffer);
		printf("发送消息:");
		scanf("%s", buffer);
		printf("\n");
		send(client, buffer, strlen(buffer), 0);
 
		if(strcmp(buffer, "quit") == 0)break;
		}
	}
close(serverSocket);
return 0;
}

