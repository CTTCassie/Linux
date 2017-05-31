#include<stdio.h>
#include<pthread.h>
#include <semaphore.h>
#define _SIZE_ 5

int buf[_SIZE_]; //使用环形数组来模拟交易场所
sem_t blanks;    //格子信号量
sem_t datas;     //数据信号量

pthread_mutex_t prolock=PTHREAD_MUTEX_INITIALIZER; //生产者之间的锁
pthread_mutex_t conlock=PTHREAD_MUTEX_INITIALIZER; //消费者之间的锁

void *product_run(void *arg) //生产者关心的是否存在空格子资源
{
	static int i=0;
	while(1)
	{
		sem_wait(&blanks); //相当于P操作,格子数-1
		pthread_mutex_lock(&prolock);//格子数不为0则加锁
		buf[i]=rand()%100;
		printf("producter is producting,i is %d,data is %d\n",i,buf[i]);
		i++;
		i %= _SIZE_;
		sem_post(&datas);  //相当于V操作,数据+1
		sleep(1);
		pthread_mutex_unlock(&prolock); //解锁
	}
}

void *consume_run(void *arg) //消费者关心是否存在数据资源
{
	static int i=0;
	while(1)
	{
		sem_wait(&datas);  //相当于P操作,数据-1
		pthread_mutex_lock(&conlock); //尚有数据未消费则加锁
		int data=buf[i];
		printf("consumer is consuming,i is %d,data is %d\n",i,data);
		i++;
		i %= _SIZE_;
		sem_post(&blanks); //相当于V操作,格子数+1
		//sleep(1);
		pthread_mutex_unlock(&conlock); //解锁
	}
}

int main()
{
	sem_init(&blanks,0,_SIZE_);
	sem_init(&datas,0,0);
	pthread_t producter1,producter2,producter3,producter4;
	pthread_t consumer1,consumer2,consumer3,consumer4;

	pthread_create(&producter1,NULL,product_run,NULL);
	pthread_create(&producter2,NULL,product_run,NULL);
	pthread_create(&producter3,NULL,product_run,NULL);
	pthread_create(&producter4,NULL,product_run,NULL);
	pthread_create(&consumer1,NULL,consume_run,NULL);
	pthread_create(&consumer2,NULL,consume_run,NULL);
	pthread_create(&consumer3,NULL,consume_run,NULL);
	pthread_create(&consumer4,NULL,consume_run,NULL);

	pthread_join(producter1,NULL);
	pthread_join(producter2,NULL);
	pthread_join(producter3,NULL);
	pthread_join(producter4,NULL);
	pthread_join(consumer1,NULL);
	pthread_join(consumer2,NULL);
	pthread_join(consumer3,NULL);
	pthread_join(consumer4,NULL);
	sem_destroy(&blanks);
	sem_destroy(&datas);
	return 0;
}
