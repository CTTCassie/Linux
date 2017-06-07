#include"comm.h"

void testserver()
{
	int shm_id=CreatShm();
	printf("shm_id=%d\n",shm_id);
	char *mem=(char *)shmat(shm_id,NULL,0);
	while(1)
	{
		sleep(1);
		printf("%s\n",mem);
	}
	shmdt(mem);
	DestroyShm(shm_id);
}

int main()
{
	testserver();
	return 0;
}
