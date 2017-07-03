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
	printf("Usage:%s [local_ip] [local_port]\n",proc);
}

int main(int argc,char *argv[])
{
	if(argc != 3){
		Usage(argv[0]);
		return 1;
	}
	int sock=socket(AF_INET,SOCK_DGRAM,0);  //创建udp协议的套接字
	if(sock < 0){
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(argv[2]));
	local.sin_addr.s_addr=inet_addr(argv[1]);   //inet_addr()将点分十进制的IP地址转变为整形，并将其转换为网络序列

	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0){ //服务器端绑定自己的IP地址和端口号
		perror("bind");
		exit(3);
	}

	while(1)
	{
		//服务器端，先recvfrom收数据，再sendto发数据
		char buf[1024];
		struct sockaddr_in client;
		socklen_t len=sizeof(client);

		ssize_t s=recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr *)&client,&len);
		if(s > 0){   //recvfrom成功
			buf[s]='\0';
			printf("[%s][%u]# %s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
			sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&client,len);
		}
		else if(s == 0){   //对端连接关闭
			printf("client exit\n");
			break;
		}
		else{    //recvfrom失败
			perror("recvfrom");
			break;
		}
	}
	close(sock);
	return 0;
}
