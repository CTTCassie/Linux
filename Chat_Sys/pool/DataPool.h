#ifndef _DATA_POOL_
#define _DATA_POOL_
#include<iostream>
#include<vector>
#include<semaphore.h>

class data_pool
{
public:
	data_pool(int cap);
	void getData(std::string &inString);
	void putData(std::string &outString);
	~data_pool();
private:
	int _cap;
	int _Product_post;
	int _Consume_post;
	std::vector<std::string> _datapool;
	sem_t _blank;
	sem_t _data;
	int _size;
};

#endif
