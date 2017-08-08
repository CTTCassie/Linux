#include"httpd.h"

static void Usage(const char *proc)
{
	printf("Usage:%s [local_ip] [local_port]\n",proc);
}

static void *accept_request(void *arg)
{
	int sock=(int )arg;
	pthread_detach(pthread_self());

	return (void *)handler_sock(sock);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	int listenfd=StartUp(argv[1],atoi(argv[2]));

	//daemon(1,0);

	while(1)
	{
		struct sockaddr_in remote;
		socklen_t len=sizeof(remote);
		int sock=accept(listenfd,(struct sockaddr *)&remote,&len);
		if(sock == -1)
		{
			print_log("accept failed",FATAL);
			continue;
		}
		printf("get a connect,ip:%s,port:%d\n",inet_ntoa(remote.sin_addr),remote.sin_port);

		pthread_t tid;

		pthread_create(&tid,NULL,accept_request,(void *)sock);
	}
	return 0;
}
