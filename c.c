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
int Cli_socket = socket(AF_INET, SOCK_STREAM, 0);   //创建和服务器连接套接字
struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));//initialize clear 0
    addr.sin_family = AF_INET;  /* Internet地址族 */
    addr.sin_port = htons(8048);  /* 端口号 */
    addr.sin_addr.s_addr = htonl("127.0.0.1");   /* IP地址 */
    inet_aton("127.0.0.1", &(addr.sin_addr));
int addrlen = sizeof(addr);
int Conn_socket =  connect(Cli_socket , (struct sockaddr *)&addr, addrlen);  //连接服务器
	printf("connected!"); 
    char cmd[1024];
    int end = 0;
    while(1)        //向服务器发送数据
    {
        if(end == 1)  break;
        memset(cmd, 0, sizeof(cmd));
        printf("please input command:\n");
        gets(cmd);
        if(strcmp(cmd,"quit") == 0 ){
                  end = 1;}
		else{
            write(Cli_socket ,cmd,strlen(cmd));
        }
    }
 
    close(Conn_socket );
}

