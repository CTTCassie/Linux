#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<pthread.h>

static void Usage(const char *proc)
{
	printf("Usage:%s [local_ip] [local_port]\n",proc);
}

int StartUp(const char *ip,int port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0); //创建tcp的套接字
	if(sock < 0){
		perror("socket");
		exit(2);
	}
	struct sockaddr_in local;
	local.sin_family=AF_INET;    //协议家族是IPv4
	local.sin_port=htons(port);   
	local.sin_addr.s_addr=inet_addr(ip); //将点分十进制的字符串转成4字节的网络序列
	
	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0){
		perror("bind");
		exit(3);
	}
	if(listen(sock,5) < 0){
		perror("listen");
		exit(4);
	}
	return sock;
}

void *request(void *arg)
{
	int new_fd=(int)arg;   //获得新连接的文件描述符
	while(1)
	{
		//先读再写
		char buf[1024];
		ssize_t s=read(new_fd,buf,sizeof(buf)-1);
		if(s > 0){   //读取成功
			buf[s]='\0';
			printf("client# %s\n",buf);
			write(new_fd,buf,strlen(buf));
		}
		else if(s == 0){ //读到文件结尾
			printf("client exit\n");
			break;
		}
		else{   //读取失败
			perror("read");
			break;
		}
	}
	close(new_fd);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	int listen_fd=StartUp(argv[1],atoi(argv[2]));  //监听套接字，监听是否有新的连接到达
	while(1)
	{
		struct sockaddr_in client;
		socklen_t len=sizeof(client);
		int new_fd=accept(listen_fd,(struct sockaddr *)&client,&len); //处理数据的真正的文件描述符
		if(new_fd < 0){  //accept失败则再次重新监听
			perror("accept");
			continue;
		}

		printf("get a connect,ip is %s,port is %u\n",\
				inet_ntoa(client.sin_addr),ntohs(client.sin_port));

		pthread_t tid;
		pthread_create(&tid,NULL,request,(void *)new_fd);

		pthread_detach(tid);  //线程分离
	}
	close(listen_fd);
	return 0;
}
