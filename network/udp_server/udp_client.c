#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>

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

	int sock=socket(AF_INET,SOCK_DGRAM,0);

	while(1)    //客户端做的操作是先从标准输入获取数据，再将数据发送到网络上，最后从网络中读取数据实现回显功能
	{
		char buf[1024];
		struct sockaddr_in server;   //server服务器的ip和port
		server.sin_family=AF_INET;
		server.sin_port=htons(atoi(argv[2]));
		server.sin_addr.s_addr=inet_addr(argv[1]);
		socklen_t len=sizeof(server);

		printf("Please Enter#");
		fflush(stdout);
		ssize_t s=read(0,buf,sizeof(buf)-1);
		if(s > 0){
			buf[s-1]='\0';
			sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&server,len);

			struct sockaddr_in remote;   //remote也是指的是服务器端
			socklen_t size=sizeof(remote);

			ssize_t _s=recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr *)&remote,&size);
			if(_s > 0){
				buf[_s]='\0';   
				printf("server echo# %s\n",buf);
			}
		}
	}
	close(sock);
	return 0;
}
