#include"DataPool.h"

data_pool::data_pool(int cap)
	:_cap(cap)
	,_Product_post(0)
	,_Consume_post(0)
	,_datapool(cap)
	,_size(0)
{
	sem_init(&_blank,0,cap);
	sem_init(&_data,0,0);
}


void data_pool::getData(std::string &inString)
{
	sem_wait(&_data);
	inString=_datapool[_Consume_post];
	_Consume_post++;
	_size--;
	_Consume_post%=_cap;
	sem_post(&_blank);
}

void data_pool::putData(std::string &outString)
{
	sem_wait(&_blank);
	_datapool[_Product_post]=outString;
	_Product_post++;
	_size++;
	_Product_post%=_cap;
	sem_post(&_data);
}

data_pool::~data_pool()
{
	_cap=0;
	_size=0;
	sem_destroy(&_blank);
	sem_destroy(&_data);
}
