#ifndef _DATA_POOL_
#define _DATA_POOL_
#include<iostream>
#include<vector>
#include<string>
#include<semaphore.h>

class data_pool
{
public:
	data_pool(int cap);
	void getData(std::string &inString);   //获取数据
	void putData(std::string &outString);  //放数据
	~data_pool();
private:
	int _cap;
	int product_post;
	int consume_post;
	std::vector<std::string> _datapool; //交易场所
	sem_t _blank;  //生产者关心的是空的格子
	sem_t _data;   //消费者关心的是是否存在数据
	int _size;
};

#endif
