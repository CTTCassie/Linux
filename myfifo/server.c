#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

void testserver()
{
	int namepipe=mkfifo("myfifo",S_IFIFO|0666);  //创建一个存取权限为0666的命名管道
	if(namepipe == -1){
		perror("mkfifo error");
		exit(1);
	}
	int fd=open("./myfifo",O_RDWR);  //打开该命名管道
	if(fd == -1){
		perror("open error");
		exit(2);
	}
	char buf[1024];
	while(1)
	{
		printf("sendto# ");
		fflush(stdout);
		ssize_t s=read(0,buf,sizeof(buf)-1); //从标准输入获取消息
		if(s > 0){
			buf[s-1]='\0';     //过滤掉从标准输入中获取的换行
			if(write(fd,buf,s) == -1){   //把该消息写入到命名管道中
				perror("write error");
				exit(3);
			}
		}
	}
	close(fd);
}

int main()
{
	testserver();
	return 0;
}
