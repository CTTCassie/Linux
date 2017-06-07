#include"comm.h"

static commSemSet(int num,int flag)
{
	key_t key=ftok(PATHNAME,PROJID);
	if(key == -1)
	{
		perror("ftok error");
		exit(1);
	}
	int sem_id=semget(key,num,flag);
	if(sem_id == -1)
	{
		perror("semget error");
		exit(2);
	}
	return sem_id;
}

int CreateSemSet(int num)
{
	return commSemSet(num,IPC_CREAT|IPC_EXCL|0666);
}

int InitSem(int sem_id,int which)
{
	union semun un;
	un.val=1;
	int ret=semctl(sem_id,which,SETVAL,un);
	if(ret < 0)
	{
		perror("semctl");
		return -1;
	}
	return 0;
}

int GetSemSet()
{
	return commSemSet(0,IPC_CREAT);
}

static int SemOp(int sem_id,int which,int op)
{
	struct sembuf buf;
	buf.sem_num=which;
	buf.sem_op=op;
	buf.sem_flg=0; //
	int ret=semop(sem_id,&buf,1);
	if(ret < 0)
	{
		perror("semop error");
		return -1;
	}
	return 0;
}

int P(int sem_id,int which)
{
	return SemOp(sem_id,which,-1);
}

int V(int sem_id,int which)
{
	return SemOp(sem_id,which,1);
}

int DestroySemSet(int sem_id)
{
	int ret=semctl(sem_id,0,IPC_RMID);
	if(ret < 0)
	{
		perror("semctl error");
		return -1;
	}
	return 0;
}
