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
#define SERVER_PORT 8048
 
/*
连接到服务器后，会不停循环，等待输入，
输入quit后，断开与服务器的连接
*/
 
int main(){
 
//客户端只需要一个套接字文件描述符，用于和服务器通信
    int clientSocket;
//描述服务器的socket
    struct sockaddr_in serverAddr;
    char sendbuf[200];
    char recvbuf[200];
    int iDataNum;
	char p[10], g[10];
	char B, yb[10];
	char key[10];
    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

	perror("socket");
	return 1;}

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    //指定服务器端的ip，本地测试：127.0.0.1
    //inet_addr()函数，将点分十进制IP转换成网络字节序IP
 
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
	strncpy(p, recvbuf, strlen(p));
	sendbuf[0] = '\0';

	// send g to server
	printf("input g:%s", sendbuf);
	gets(sendbuf);
	send(clientSocket, sendbuf, strlen(sendbuf), 0);
	strncpy(g, sendbuf, strlen(g));
	
	printf("p is %s, g is %s\n", p, g);

	// recv yb
	memset(recvbuf, '\0', sizeof(recvbuf));
	printf("waiting for yb from server!\n");
	recv(clientSocket, recvbuf, 200, 0);
    strncpy(yb, recvbuf, strlen(yb));
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

	//sendbuf[0] = '\0';
	//recvbuf[0] = '\0';
	//recv(clientSocket, recvbuf, 200, 0);
	//strncpy(yb, recvbuf, strlen(yb));

	double t_key;
   	//t_key = atof(t_key);
	t_key = fmod(pow(atof(yb), t_b), atof(p));
	printf("final key is: %lf\n", t_key);


    while(1){
		printf("发送消息:");
		scanf("%s", sendbuf);
		printf("\n");
		send(clientSocket, sendbuf, strlen(sendbuf), 0);
		
		if(strcmp(sendbuf, "quit") == 0)break;
		
		printf("读取消息:");
		recvbuf[0] = '\0';
		iDataNum = recv(clientSocket, recvbuf, 200, 0);
		recvbuf[iDataNum] = '\0';
		printf("%s\n", recvbuf);
	}
    
	close(clientSocket);
    return 0;
}
