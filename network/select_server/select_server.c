#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int fds[sizeof(fd_set)*8];   //定义全局的数组，用来存储文件描述符

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

	//setsockopt端口号的复用
	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0){ //服务器绑定自己的ip地址和端口号
		perror("bind");
		exit(3);
	}

	if(listen(sock,10) < 0){  //将sock变为监听套接字
		perror("listen");
		exit(4);
	}
	return sock;
}

int main(int argc,char *argv[])
{
	if(argc != 3){
		Usage(argv[0]);
		return 1;
	}

	int listen_fd=StartUp(argv[1],atoi(argv[2]));

	printf("listen_fd is %d\n",listen_fd);

	int i=0;
	int nums=sizeof(fds)/sizeof(fds[0]);
	for(;i<nums;i++){  //给全局数组fds赋初值
		fds[i]=-1;
	}

	fds[0]=listen_fd;   //下标为0的位置存储监听套接字

	while(1)
	{
		int maxfd=-1;  //最大文件描述符值
		struct timeval timeout={2,0};  //设置超时时间,每隔两秒钟记录一次超时时间
		//struct timeval timeout={0,0};    //以轮询的方式检测
		fd_set rfds;   //读文件描述符集
		FD_ZERO(&rfds);//清空rfds里面的所有文件描述符集
		for(i=0;i<nums;i++)
		{
			if(fds[i] >= 0){     //置位，说明此时有连接到来
				FD_SET(fds[i],&rfds);
			}
			if(fds[i] > maxfd){  //更新最大的文件描述符的值
				maxfd=fds[i];
			}
		}
		switch(select(maxfd+1,&rfds,NULL,NULL,&timeout))
		{
			case -1:    //select失败
				perror("select");
				break;
			case 0:
				printf("time out...\n");
				break;
			default:
				{
					for(i=0;i<nums;i++){
				    	if(i == 0 && FD_ISSET(fds[i],&rfds)){   //监听套接字获得新连接
				    		struct sockaddr_in client;
				    		socklen_t len=sizeof(client);
				    		int new_fd=accept(listen_fd,(struct sockaddr *)&client,&len);
				    		if(new_fd < 0){
				    			perror("accept");
				    			exit(5);
				    		}

				    		printf("get a new connection,ip is %s,port is %d\n",\
				    				inet_ntoa(client.sin_addr),ntohs(client.sin_port));

				    		//accpet成功，将new_fd添加到文件描述符数组中
				    		int j=1;
				    		for(;j<nums;j++){
				    			if(fds[j] == -1){
				    				fds[j]=new_fd;
									break;
				    			}
				    		}

				    		//文件描述符数组中没有空闲的位置可被使用
				    		if(j == nums){  
				    			close(new_fd);
				    		}
				    	}
				    	else if(i != 0 && FD_ISSET(fds[i],&rfds)){  //普通事件就绪
							//I/O的数据等待是给select做的,程序运行到这里一定有事件就绪
							while(1){
				    	    	char buf[1024];
				    	    	ssize_t s=read(fds[i],buf,sizeof(buf)-1);
				    	    	if(s > 0){   //读成功
				    	    		buf[s-1]='\0';
				    	    		printf("client# %s\n",buf);
				    	    	}
				    	    	else if(s == 0){ //有客户端退出
				    	    		printf("client exit\n");
				    	    		close(fds[i]);
				    	    		fds[i]=-1;
									break;
				    	    	}
				    	    	else{  //读失败
				    	    		perror("read");
				    	    		close(fds[i]);
				    	    		fds[i]=-1;
									break;
				    	    	}
							}
				    	}
				    	else{   //没有链接就绪
				    		continue;
				    	}
					}
					break;
				}

		}
	}
	close(listen_fd);
	return 0;
}
