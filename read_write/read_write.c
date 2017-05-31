#include<stdio.h>
#include<pthread.h>

int count=0;
pthread_rwlock_t myrwlock;

void *reader_run(void *arg)
{
	while(1)
	{
		sleep(1);
		int ret=pthread_rwlock_rdlock(&myrwlock);
		if(ret != 0)
		{
			printf("the lock is user by writer\n");
			continue;
		}
		printf("reader is %lu,count=%d\n",pthread_self(),count);
		pthread_rwlock_unlock(&myrwlock);
	}
}

void *writer_run(void *arg)
{
	while(1)
	{
		sleep(1);
		int ret=pthread_rwlock_wrlock(&myrwlock);
		if(ret != 0)
		{
			printf("the lock is used by reader\n");
			continue;
		}
		count++;
		printf("writer is %lu,count=%d\n",pthread_self(),count);
		pthread_rwlock_unlock(&myrwlock);
	}
}

int main()
{
	pthread_rwlock_init(&myrwlock,NULL);
	pthread_t reader;
	pthread_t writer;
	pthread_create(&reader,NULL,reader_run,NULL);
	pthread_create(&writer,NULL,writer_run,NULL);
	pthread_join(reader,NULL);
	pthread_join(writer,NULL);
	pthread_rwlock_destroy(&myrwlock);
	return 0;
}
