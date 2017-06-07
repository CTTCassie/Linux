#include"comm.h"

static int CommShm(int flag)
{
	key_t key=ftok(PATHNAME,PROCID);
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}
	int shm_id=shmget(key,SIZE,flag);
	if(shm_id < 0)
	{
		perror("shmget");
		return -2;
	}
	return shm_id;
}

int CreatShm()
{
	return CommShm(IPC_CREAT|IPC_EXCL|0666);
}

int GetShm()
{
	return CommShm(IPC_CREAT);
}

//int AtShm();
//int DtShm();

int DestroyShm(int shm_id)
{
	int ret=shmctl(shm_id,IPC_RMID,NULL);
	if(ret < 0)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}
