#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

static void Usage(const char *proc)
{
	printf("Usage:%s [server_ip] [server_port]\n",proc);
}

int main(int argc,char *argv[])
{
	if(argc != 3){
		Usage(argv[0]);
		return 1;
	}
	int sock=socket(AF_INET,SOCK_STREAM,0);  //创建套接字
	if(sock < 0){
		perror("socket");
		return 2;
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	//与服务器端建立连接
	if(connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0){
		perror("connect");
		return 3;
	}
	while(1)
	{
		//先写再读
		printf("client# ");
		fflush(stdout);
		char buf[1024];
		ssize_t s=read(0,buf,sizeof(buf)-1); //从标准输入中读取数据
		if(s > 0){
			buf[s-1]='\0';
			write(sock,buf,strlen(buf));  //将数据写入到网络套接字sock中去
			ssize_t _s=read(sock,buf,sizeof(buf)-1); //从网络中获得数据，实现回显功能
			if(_s > 0){
				buf[_s]='\0';
				printf("server echo# %s\n",buf);
			}
		}
		else if(s == 0){
			printf("read ending\n");
			exit(4);
		}
		else{
			perror("read");
			break;
		}
	}
	return 0;
}
