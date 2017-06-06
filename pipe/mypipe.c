#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int _pipe[2]={0,0};
	int ret=pipe(_pipe);  //创建管道
	if(ret == -1)
	{
		perror("create pipe error");
		return 1;
	}
	printf("_pipe[0] is %d,_pipe[1] is %d\n",_pipe[0],_pipe[1]);
	pid_t id=fork();     //父进程fork子进程
	if(id < 0)
	{
		perror("fork error");
		return 2;
	}
	else if(id == 0)  //child,写
	{
		printf("child writing\n");
		close(_pipe[0]);
		int count=5;
		const char *msg="i am from XATU";
		while(count--)
		{
			write(_pipe[1],msg,strlen(msg));
			sleep(1);
		}
		close(_pipe[1]);
		exit(1);
	}
	else  //father,读
	{
		printf("father reading\n");
		close(_pipe[1]);
		char msg[1024];
		int count=5;
		while(count--)
		{
			ssize_t s=read(_pipe[0],msg,sizeof(msg)-1);
			if(s > 0){
				msg[s]='\0';
				printf("client# %s\n",msg);
			}
			else{
				perror("read error");
				exit(1);
			}
		}
		if(waitpid(id,0,NULL) != -1){
			printf("wait success\n");
		}
	}
	return 0;
}
