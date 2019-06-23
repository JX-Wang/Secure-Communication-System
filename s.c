#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
int Ser_socket = socket(AF_INET, SOCK_STREAM, 0);   //创建一个负责监听的套接字
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr)); //initialize addr
    addr.sin_family = AF_INET;  /* Internet地址族 */
    addr.sin_port = htons(8048);  /* 端口号,随意 */
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   /* IP地址 */
int resp = bind(Ser_socket , (struct sockaddr *)&addr, sizeof(addr));  //连接
int lresp = listen(Ser_socket , 20);   
 
while(1){
    struct sockaddr_in cliaddr;
    int client_socket = accept(Ser_socket , (struct sockaddr *)&cliaddr , sizeof(cliaddr));
	printf("client connected!");
    char cmd[1024];
	int ret = read(client_socket, cmd, 1023);	
	cmd[ret]='\0';
	printf("recv msg: %s", cmd);
//处理指令；
    if(strcmp(cmd,"quit") == 0){
        break; 
    }
    }
close(Ser_socket );
}
