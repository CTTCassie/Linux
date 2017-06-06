#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

void testclient()
{
	int fd=open("./myfifo",O_RDWR);
	if(fd == -1){
		perror("open error");
		exit(1);
	}
	char buf[1024];
	while(1){
		ssize_t s=read(fd,buf,sizeof(buf)-1);
		if(s > 0){
			printf("RecvFrom# %s\n",buf);
		}
		else{  //读失败或者是读取到字符结尾
			perror("read error");
			exit(2);
		}
	}
	close(fd);
}

int main()
{
	testclient();
	return 0;
}
