#include"DataPool.h"

data_pool::data_pool(int cap)
	:_cap(cap)
	,product_post(0)
	,consume_post(0)
	,_datapool(cap)
	,_size(0)
{
	sem_init(&_blank,0,cap); //格子变量初始化为容量
	sem_init(&_data,0,0); //数据变量初始化为0
}

void data_pool::getData(std::string &inString)
{
    //消费者关心的是数据资源
	sem_wait(&_data);  //P操作
	inString=_datapool[consume_post];
	consume_post++;
	_size--;
	consume_post%=_cap;
	sem_post(&_blank); //V操作
}

void data_pool::putData(std::string &outString)
{
    //生产者关心的是数据资源
	sem_wait(&_blank); //P操作
	_datapool[product_post]=outString;
	product_post++;
	_size++;
	product_post%=_cap;
	sem_post(&_data); //V操作
}

data_pool::~data_pool()
{
	_cap=0;
	_size=0;
	sem_destroy(&_blank);
	sem_destroy(&_data);
}
