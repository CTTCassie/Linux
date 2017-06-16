#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

int StartUp(int port,char *ip)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1)
	{
		perror("socket");
		exit(1);
	}

	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	socklen_t len=sizeof(local);
	int b=bind(sock,(const struct sockaddr *)&local,len);
	if(b == -1)
	{
		perror("bind");
		exit(2);
	}
	if(listen(sock,5) == -1)
	{
		perror("listen");
		exit(3);
	}
	return sock;
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("input error! %s [ip] [port]\n",argv[0]);
		return 4;
	}
	int listenfd=StartUp(atoi(argv[2]),argv[1]);
	while(1)
	{
		struct sockaddr_in remote;
		int len=sizeof(struct sockaddr_in);
		int a=accept(listenfd,(struct sockaddr *)&remote,&len);
		if(a == -1)
		{
			perror("accept");
			continue;
		}
		printf("connect success,ip is %s,port is %u\n",\
				inet_ntoa(remote.sin_addr),ntohs(remote.sin_port));
		pid_t id=fork();
		if(id < 0)   //fork error
		{
			perror("fork");
			close(a);
			continue;
		}
		else if(id == 0)  //child
		{
			close(listenfd);
			if(fork() > 0)
			{
				exit(1);
			}
			while(1)
			{
				char buf[1024];
				ssize_t size=read(a,buf,sizeof(buf)-1);
				if(size > 0)
				{
					buf[size]='\0';
					printf("client#%s\n",buf);
				}
				else if(size == 0)
				{
					printf("read ending,code exit\n");
					break;
				}
			}
			exit(1);
		}
		else   //father
		{
			close(a);
			waitpid(id,NULL,0);
		}
	}
	close(listenfd);
	return 0;
}
