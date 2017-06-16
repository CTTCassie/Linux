#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("input error! %s [ip] [port]\n",argv[0]);
		return 3;
	}
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1)
	{
		perror("socket");
		return 1;
	}
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr=inet_addr(argv[1]);
	int c=connect(sock,(const struct sockaddr *)&server,sizeof(server));
	if(c == -1)
	{
		perror("connect");
		return 2;
	}
	while(1)
	{
		printf("send#");
		fflush(stdout);
		char buf[1024];
		ssize_t size=read(0,buf,sizeof(buf)-1);
		buf[size-1]='\0';
		if(size > 0)
		{
			if(write(sock,buf,size) == -1)
			{
				perror("write");
				return 4;
			}
		}
	}
	close(sock);
	return 0;
}
