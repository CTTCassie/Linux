#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mylock=PTHREAD_MUTEX_INITIALIZER;  //互斥锁
pthread_cond_t mycond=PTHREAD_COND_INITIALIZER;    //条件变量--同步

typedef struct node_list   //一个交易场所
{
	int _data;
	struct node_list *_next;
}node,*node_p,**node_pp;

node_p head;

static node_p alloc_node(int data)
{
	node_p NewNode=(node_p)malloc(sizeof(node));
	if(NewNode == NULL)
	{
		perror("malloc");
		exit(1);
	}
	NewNode->_next=NULL;
	NewNode->_data=data;
	return NewNode;
}

static void free_node(node_p del)
{
	if(del)
	{
		free(del);
		del=NULL;
	}
}

void initList(node_pp h)
{
	*h=alloc_node(0);
}

void pushHead(node_p h,int data)
{
	node_p NewNode=alloc_node(data);
	NewNode->_next=h->_next;
	h->_next=NewNode;
}

int IsEmpty(node_p h)
{
	return h->_next == NULL ? 1: 0;
}

void popHead(node_p h,int *data)
{
	if(!IsEmpty(h))
	{
		node_p del=h->_next;
		h->_next=del->_next;
		*data=del->_data;
		free_node(del);
	}
}

void showList(node_p h)
{
	node_p cur=h->_next;
	while(cur)
	{
		printf("%d ",cur->_data);
		cur=cur->_next;
	}
	printf("\n");
}

void DestroyList(node_p h)
{
	int data=0;
	if(!IsEmpty(h))
	{
		popHead(h,&data);
	}
	free_node(h);
}


void *product_run(void *arg)  //生产者往链表中头插数据
{
	int data=0;
	node_p h=(node_p)arg;
	while(1)
	{
		usleep(100000);
		data=rand()%1000;
		pthread_mutex_lock(&mylock);   //加锁
		pushHead(h,data);
		pthread_mutex_unlock(&mylock); //解锁
		pthread_cond_signal(&mycond);  //唤醒等待的线程
		printf("product is done...\n");
	}
}

void *consumer_run(void *arg)  //消费者从链表中头删数据
{
	int data=0;
	node_p h=(node_p)arg;
	while(1)
	{
		pthread_mutex_lock(&mylock);  //加锁
		if(IsEmpty(h))  //当交易场所为空的时候，等待并归还所获得的锁
		{
			pthread_cond_wait(&mycond,&mylock);
		}
		popHead(h,&data);
		pthread_mutex_unlock(&mylock);//解锁
		printf("consumer is done,data is %d\n",data);
	}
}

int main()
{
	initList(&head);
	pthread_t product;
	pthread_t consumer;
	pthread_create(&product,NULL,product_run,(void *)head);
	pthread_create(&consumer,NULL,consumer_run,(void *)head);
	pthread_join(product,NULL);
	pthread_join(consumer,NULL);
	DestroyList(head);

	pthread_mutex_destroy(&mylock);
	pthread_cond_destroy(&mycond);
//	int i=0;
//	for(;i<10;i++)
//	{
//		pushHead(head,i);
//		showList(head);
//		sleep(1);
//	}
//	int data=0;
//	for(;i>5;i--)
//	{
//		popHead(head,&data);
//		showList(head);
//		sleep(1);
//	}
//	DestroyList(head);
	return 0;
}
