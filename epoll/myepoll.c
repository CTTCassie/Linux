#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#define _SIZE_ 10240
//创建一个结构体类型，里面保存一个文件描述符和一个缓冲区
typedef struct epbuf
{
	int fd;
	char buf[_SIZE_];
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
	if(sock == -1)
	{
		perror("socket error");
		exit(2);
	}

	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(const struct sockaddr *)&local,sizeof(local)) == -1)
	{
		perror("bind error");
		exit(3);
	}
	if(listen(sock,5) == -1)
	{
		perror("listen error");
		exit(4);
	}
	return sock;
}

static void Usage(char *proc)
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

	int epfd=epoll_create(256);

	if(epfd == -1)
	{
		perror("epoll_create error");
		return 5;
	}
	struct epoll_event ev;
	ev.events=EPOLLIN | EPOLLET;
	ev.data.ptr=alloc_epbuf(listenfd);
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);  //events register
	while(1)
	{
		struct epoll_event evs[32];
		int max_evs=32;
	    int timeout=-1;   //-1 wait by block,设置超时时间
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
					for(;i<nums;i++)
					{
						int fd=((epbuf_p)(evs[i].data.ptr))->fd;
						if((fd == listenfd) && (evs[i].events & EPOLLIN))
						{
							//events is listenfd and is EPOLLIN
							struct sockaddr_in peer;
							socklen_t len=sizeof(peer);
							int new_sock=accept(fd,(struct sockaddr *)&peer,&len);
							if(new_sock == -1)
							{
								perror("accept");
								continue;
							}
							ev.events=EPOLLIN;    //LT
	                        ev.data.ptr=alloc_epbuf(new_sock);
	                        epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
						}//if
						else if((fd != listenfd) && (evs[i].events & EPOLLIN))
						{
							char *buf=((epbuf_p)(evs[i].data.ptr))->buf;
							ssize_t size=read(fd,buf,_SIZE_-1);
							if(size > 0)         //read successful
							{
								buf[size-1]='\0';
								printf("client# %s\n",buf);
								ev.events=EPOLLOUT;    //LT
	                            epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
							}
							else if(size == 0)   //read ending
							{
								del_epbuf(evs[i].data.ptr);
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
								close(fd);
								printf("client is quit\n");
							}
							else      //read error
							{
								perror("read");
							}
						}//else if
						else if((fd != listenfd) && (evs[i].events & EPOLLOUT))
						{
							char *msg="HTTP/1.0 200 OK\r\n\r\n  \
											 <html><h2>hello everyone!</h2></html>\n";
							ssize_t ret=write(fd,msg,strlen(msg));
							if(ret == -1)
							{
								perror("write");
								return 6;
							}
							del_epbuf(evs[i].data.ptr);
							epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
							close(fd);
						}
						else   //do otherthings
						{}
					}//for
				}
				break;
		}
	}
	return 0;
}
