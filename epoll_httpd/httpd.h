#ifndef _HTTPD_
#define _HTTPD_
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/sendfile.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<strings.h>
#include<string.h>
//#include<pthread.h>
#define NORMAL 0
#define WARNING 1 
#define FATAL 2
#define SIZE 4096

typedef struct epbuf
{
    int fd;
    char buf[SIZE];
}epbuf_t,*epbuf_p,**epbuf_pp;

void print_log(const char *log_msg,int level);
int StartUp(const char *ip,int port);
int handler_sock(int sock);

#endif   // _HTTPD_
