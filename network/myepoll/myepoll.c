#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#define SIZE 10240

//创建一个结构体类型，里面保存一个文件描述符和一段缓冲区
typedef struct epbuf
{
	int fd;
	char buf[SIZE];
}epbuf_t,*epbuf_p,**epbuf_pp;

static epbuf_p alloc_epbuf(int fd)
{
	epbuf_p ptr=(epbuf_p)malloc(sizeof(epbuf_t));
	if(NULL == ptr)
	{
		perror("malloc");
		exit(6);
	}
	ptr->fd=fd;
	return ptr;
}

static void del_epbuf(epbuf_p ptr)
{
	if(NULL != ptr)
	{
		free(ptr);
		ptr=NULL;
	}
}

int StartUp(const char *ip,int port)
{
	assert(ip);
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket error");
		exit(2);
	}
	//端口号复用
	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0)
	{
		perror("bind error");
		exit(3);
	}

	if(listen(sock,10) < 0)
	{
		perror("listen error");
		exit(4);
	}
	return sock;
}

int myread(int sock,char *buf)
{
	int len=0;
	int total=0;
	while((len=read(sock,buf+total,1024) > 0) && (len == 1024))
	{
		//读成功并且读到的值与期望值相符合
		total += len;
	}
	//没有读到1024个字节，更新total
	if(len > 0 && len < 1024){
		total += len;
	}
	buf[total]='\0';
	return total;
}

int set_fd_nonblock(int sock)  //设置非阻塞
{
	int old_opt=fcntl(sock,F_GETFL);
	int new_opt=old_opt | O_NONBLOCK;
	fcntl(sock,F_SETFL,new_opt);
	return old_opt;
}

static void Usage(const char *proc)
{
	printf("Usage:%s [local_ip] [local_port]\n",proc);
}

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		return 1;
	}
	int listenfd=StartUp(argv[1],atoi(argv[2]));

	int epfd=epoll_create(256);  //创建一个epoll模型，其实底层就是创建一颗红黑树和就绪队列

	if(epfd < 0)
	{
		perror("epoll_create error");
		return 5;
	}

	struct epoll_event ev;
	ev.events=EPOLLIN | EPOLLET;   //工作模式是ET模式
	ev.data.ptr=alloc_epbuf(listenfd);

	set_fd_nonblock(listenfd);      //如果工作在ET模式则必须设置非阻塞
		
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);  //将监听套接字添加到epoll模型中

	while(1)
	{
		struct epoll_event evs[32];
		int max_evs=32;
	    int timeout=1000;   //设置超时时间
		//int timeout=-1;   //此时说明阻塞
	    int nums=0;
	    switch(nums=epoll_wait(epfd,evs,max_evs,timeout))
		{
			case -1:
				perror("epoll_wait error");
			    break;
			case 0:
				printf("timeout...\n");
			    break;
			default:
				{
					int i=0;
					for(;i<nums;i++){
						int sock=((epbuf_p)(evs[i].data.ptr))->fd;
						if((sock == listenfd) && (evs[i].events & EPOLLIN)){
							//监听事件到来并且发生了EPOLLIN事件
							struct sockaddr_in client;
							socklen_t len=sizeof(client);
							int new_sock=accept(sock,(struct sockaddr *)&client,&len);
							if(new_sock < 0){
								perror("accept");
								continue;
							}

							printf("get a new client,ip is %s,port is %d\n",\
									inet_ntoa(client.sin_addr),ntohs(client.sin_port));

							ev.events=EPOLLIN | EPOLLET;    //ET

							set_fd_nonblock(new_sock);

	                        ev.data.ptr=alloc_epbuf(new_sock);
	                        epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
						}//if
						else if((sock != listenfd) && (evs[i].events & EPOLLIN)){
							//普通事件到达并且发生了EPOLLIN事件
							char *buf=((epbuf_p)(evs[i].data.ptr))->buf;
							//ssize_t s=read(sock,buf,SIZE-1);
							ssize_t s=myread(sock,buf);
							if(s > 0){  //读成功
								buf[s-1]='\0';
								printf("client# %s\n",buf);
								//修改事件的状态并将其添加到就绪队列中去
								ev.events=EPOLLOUT;    
	                            epoll_ctl(epfd,EPOLL_CTL_MOD,sock,&ev);
							}
							else if(s == 0){  //有客户端退出
								printf("client is quit\n");
								del_epbuf(evs[i].data.ptr);
								epoll_ctl(epfd,EPOLL_CTL_DEL,sock,NULL);
								close(sock);
							}
							else{  //读失败
								perror("read");
								del_epbuf(evs[i].data.ptr);
								epoll_ctl(epfd,EPOLL_CTL_DEL,sock,NULL);
								close(sock);
							}
						}//else if
						else if((sock != listenfd) && (evs[i].events & EPOLLOUT))
						{
							//是普通事件并且发生了EPOLLOUT事件
							char *msg="HTTP/1.0 200 OK\r\n<html><h1>HELLO EPOLL!!!</h1></html>\n";
							ssize_t ret=write(sock,msg,strlen(msg));
							if(ret < 0)
							{
								perror("write");
								return 6;
							}
							del_epbuf(evs[i].data.ptr);
							epoll_ctl(epfd,EPOLL_CTL_DEL,sock,NULL);
							close(sock);
						}
						else   //其他事件不做任何处理
						{}
					}//for
				}
				break;
		}
	}
	return 0;
}
