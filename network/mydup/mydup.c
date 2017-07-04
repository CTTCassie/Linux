#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	//输入重定向
	int fd=open("./log",O_RDWR|O_CREAT,0644);
	if(fd < 0){
		perror("open");
		return 1;
	}   //fd = 3
	close(1);
	//dup(fd);   //重定向,将最小的未被使用的文件描述符分配给新的文件描述符
	dup2(fd,1);  //将fd指向的文件使得1也指向它，再关闭文件描述符1所指向的文件
	int count=10;
	while(count){
		printf("hello %d\n",count);
		count--;
	}

//	umask(0);
//	close(1);
//	int fd=open("./log",O_RDWR|O_CREAT,0666);
//	int count=10;
//	while(count){
//		printf("hello %d\n",count);
//		count--;
//	}
	return 0;
}
