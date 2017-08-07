#include"UdpServer.h"
#include<unistd.h>
using namespace std;

void Usage(char *proc)
{
	printf("Usage: %s [server_ip] [server_port]\n",proc);
}

static void * run_product(void *arg)
{
	udp_server *ser=(udp_server*)arg;
	while(1)
	{
		string str;
		ser->recvData(str);
	}
	return NULL;
}

static void * run_consume(void *arg)
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
	daemon(0,0);  //使其以精灵进程的形式在后台运行
	string ip=argv[1];
	int port=atoi(argv[2]);
	udp_server server(ip,port);
	server.initServer();
    //创建生产者消费者线程
	pthread_t product; 
	pthread_t consumer;
	pthread_create(&product,NULL,run_consume,&server);
	pthread_create(&consumer,NULL,run_product,&server);
	pthread_join(product,NULL);
	pthread_join(consumer,NULL);
	return 0;
}
