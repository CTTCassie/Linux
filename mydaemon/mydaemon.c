#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>

void mydaemon()
{
	//1.将文件模式创建屏蔽字设置为0
	umask(0);
	//2.fork子进程，父进程退出
	if(fork() > 0)//father
	{
		exit(0);
	}
	//3.创建一个新会话
	setsid();
	//4.更改当前的工作目录
	chdir("/");
	//5.关闭不需要的文件描述符
	close(0);
	//close(1);
	close(2);
	//6.忽略SIGCHLD信号
	signal(SIGCHLD,SIG_IGN);
}

int main()
{
	//mydaemon();
	//daemon(0,0);   //默认更改工作目录并且更改文件描述符为空，使其指向/dev/null
	daemon(1,0);     //不更改工作目录也不更改文件描述符
	while(1)
	{
		sleep(1);
	}
	return 0;
}
