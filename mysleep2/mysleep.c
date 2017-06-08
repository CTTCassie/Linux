#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void my_handler(int signo)
{
	//自定义的处理动作为空
}

//int mysleep(int timeout)
//{
//	struct sigaction act,oact;
//	act.sa_handler=my_handler;  //用户自定义的信号处理动作
//	sigemptyset(&act.sa_mask);  //清空该信号量
//	act.sa_flags=0;
//	sigaction(SIGALRM,&act,&oact);  //注册闹钟信号处理函数
//	alarm(timeout);  //设置闹钟
//	pause();         //挂起   
//	//有可能正常返回也有可能异常返回
//	int ret=alarm(0);
//	sigaction(SIGALRM,&oact,NULL);  //恢复信号之前的处理动作
//	return ret;
//}

int mysleep(int timeout)
{
	struct sigaction act,oact;
	act.sa_handler=my_handler;  //用户自定义的信号处理动作
	sigemptyset(&act.sa_mask);  //清空该信号量
	act.sa_flags=0;
	sigaction(SIGALRM,&act,&oact);  //注册闹钟信号处理函数
	//使得alarm和pause成为原子操作
	sigset_t mask,omask,smask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGALRM);
	sigprocmask(SIG_BLOCK,&mask,&omask);

	alarm(timeout);  //设置闹钟

	smask=omask;
	sigdelset(&smask,SIGALRM);
	sigsuspend(&smask);
	int ret=alarm(0);
	sigaction(SIGALRM,&oact,NULL);  //恢复信号之前的处理动作

	sigprocmask(SIG_SETMASK,&omask,NULL);
	return ret;
}

int main()
{
	int count=0;
	while(1)
	{
		count++;
		mysleep(1);
		printf("mysleep %d\n",count);
	}
//	struct sigaction act,oact;
//	act.sa_handler=my_handler;  
//	sigemptyset(&act.sa_mask); 
//	act.sa_flags=0;
//	sigaction(2,&act,&oact);
//	while(1);
	return 0;
}
