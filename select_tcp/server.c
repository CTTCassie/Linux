#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>

#define SIZE 1024
int gfds[SIZE];

int StartUp(int port,const char *ip)
{
	assert(ip);
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
	int len=sizeof(local);
	if(bind(sock,(const struct sockaddr *)&local,len) == -1)
	{
		perror("bind");
		exit(2);
	}
	int listenfd=listen(sock,5);
	if(listenfd == -1)
	{
		perror("listen");
		exit(3);
	}
	return listenfd;
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		printf("Usage:%s [local_ip] [local_port]\n",argv[0]);
		exit(4);
	}
	int listenfd=StartUp(atoi(argv[2]),argv[1]);
	int i=0;
	for(;i<SIZE;i++)
	{
		gfds[i]=-1;     //init array invalid
	}
	gfds[0]=listenfd;   //0 is listenfd
	while(1)
	{
		int max=-1;
		struct timeval timeout={5,0};
		fd_set rfds;
		FD_ZERO(&rfds);
		for(i=0;i<SIZE;++i)
		{
			if(gfds[i] > max)
			{
				max=gfds[i];   //update max
			}
			if(gfds[i] >= 0)   //set bit
			{
				FD_SET(gfds[i],&rfds);
			}
		}
		switch(select(max+1,&rfds,NULL,NULL,&timeout))
		{
			case -1:    //error
				perror("select");
				break;
		    case 0:    //timeout
				printf("timeout ...\n");
				break;
			default:
				for(int j=0;j<SIZE;j++)
				{
					if(gfds[j] < 0)  //not request arrive
					{
						continue;
					}
					else if((gfds[j] == listenfd) && (FD_ISSET(gfds[j],&rfds)))  //is listenfd and get ready
					{
						struct sockaddr_in peer;
						int len=sizeof(peer);
						int sock=accept(listenfd,(struct sockaddr *)&peer,&len);
						if(sock == -1)
						{
							perror("accept");
							exit(5);
						}
						int k=0;
						for(;k<SIZE;++k)
						{
							if(gfds[k] == -1)   //find suitable posstion for sock
							{
								gfds[k]=sock;
								printf("exit a client,ip is %s,port is %d\n",\
										inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));
								break;
							}
						}
						if(k == SIZE)   //not have empty posstion
							close(sock);
					
					}
					else if(FD_ISSET(gfds[j],&rfds))  //get ready
					{
						char buf[128];
						while(1)
						{
							ssize_t size=read(gfds[j],buf,sizeof(buf)-1);
							if(size > 0)
							{
								buf[size-1]='\0';
								printf("client# %s\n",buf);
							}
							else if(size == 0)
							{
								close(gfds[j]);
								gfds[j]=-1;
								break;
							}
							else
							{
								perror("read");
								exit(6);
							}
						}
					}
				}
				break;
		}
	}
	close(listenfd);
	return 0;
}
