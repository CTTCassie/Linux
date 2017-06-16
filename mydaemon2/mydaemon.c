#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

void mydaemon()
{
	umask(0);

	pid_t id=fork(); //第一次fork，终止父进程
	if(id < 0){
		perror("fork1");
		return;
	}
	else if(id > 0){
		exit(0);
	}

	setsid();

	id=fork();
	if(id < 0){
		perror("fork2");
		return;
	}
	else if(id > 0){ //father，第二次fork，保证daemon进程之后不会打开tty设备
		exit(0);
	}

	chdir("/");

	close(0);
	close(1);
	close(2);
	
	signal(SIGCHLD,SIG_IGN);  //注册子进程忽略信号
}

int main()
{
	mydaemon();
	while(1)
	{
		sleep(1);
	}
	return 0;
}
