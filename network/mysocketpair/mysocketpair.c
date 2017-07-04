#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int sv[2];
	int sock=socketpair(PF_LOCAL,SOCK_STREAM,0,sv);
	if(sock < 0){
		perror("sockpair");
		return 1;
	}
	printf("sv[0]=%d,sv[1]=%d\n",sv[0],sv[1]);
	pid_t id=fork();
	if(id < 0){
		perror("fork");
		return 2;
	}
	else if(id == 0){   //child
		close(sv[0]);
		//先写后读
		char buf[1024];
		const char *msg="I am child";
		while(1)
		{
			write(sv[1],msg,strlen(msg));
	     	ssize_t s=read(sv[1],buf,sizeof(buf)-1);
	     	if(s > 0){     //读成功
	     		buf[s]='\0';
	     		printf("f-->c# %s\n",buf);
	     	}
	     	else if(s == 0){ //读到结尾
	     		printf("child exit\n");
	     		break;
	     	}
			else{   //读取失败
				perror("read");
				break;
			}
			sleep(1);
		}
		close(sv[1]);
		exit(3);   //文件描述符关闭之后子进程直接退出
	}
	else{   //father
		close(sv[1]);
		//先读后写
		while(1)
		{
			char buf[1024];
			const char *msg="I am father";
			ssize_t s=read(sv[0],buf,sizeof(buf)-1);
			if(s > 0){
				buf[s]='\0';
				printf("c-->f# %s\n",buf);
				write(sv[0],msg,strlen(msg));
			}
		}
		waitpid(id,NULL,0);
		close(sv[0]);
	}
	return 0;
}
