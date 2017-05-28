#include"UdpServer.h"
#include<unistd.h>
using namespace std;
void Usage(char *usage)
{
	printf("Usage: %s [server_ip] [server_port]\n",usage);
}

static void * proData(void *arg)
{
	udp_server *ser=(udp_server*)arg;
	while(1)
	{
		string str;
		ser->recvData(str);
//		sleep(1);
//		cout<<str<<endl;
	}

	return NULL;
}

static void * conData(void *arg)
{
	udp_server *ser=(udp_server*)arg;
	while(1)
	{
		ser->broadcast();
	}
	return NULL;
}


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		Usage(argv[0]);
	}
	
	daemon(0,0);
	string ip=argv[1];
	int port=atoi(argv[2]);
	udp_server server(ip,port);
	server.initServer();

	pthread_t product;
	pthread_t consumer;
	pthread_create(&product,NULL,proData,&server);
	pthread_create(&consumer,NULL,conData,&server);
	pthread_join(product,NULL);
	pthread_join(consumer,NULL);

	return 0;
}
