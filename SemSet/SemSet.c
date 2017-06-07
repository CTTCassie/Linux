#include"comm.h"

void testSemSet()
{
	int sem_id=CreateSemSet(1);  //创建信号量
	InitSem(sem_id,0);
	pid_t id=fork();
	if(id < 0){
		perror("fork error");
		exit(1);
	}
	else if(id == 0){   //child，打印AA
		printf("child is running,pid=%d,ppid=%d\n",getpid(),getppid());
		while(1)
		{
			P(sem_id,0);  //p操作，信号量的值减1
			printf("A");
			usleep(10031);
			fflush(stdout);
			printf("A");
			usleep(10021);
			fflush(stdout);
			V(sem_id,0); //v操作，信号量的值加1
		}
	}
	else   //father，打印BB
	{
		printf("father is running,pid=%d,ppid=%d\n",getpid(),getppid());
		while(1)
		{
			P(sem_id,0);
			printf("B");
			usleep(10051);
			fflush(stdout);
			printf("B");
			usleep(10003);
			fflush(stdout);
			V(sem_id,0);
		}
		wait(NULL);
	}
	DestroySemSet(sem_id);
}

int main()
{
	testSemSet();
	return 0;
}
