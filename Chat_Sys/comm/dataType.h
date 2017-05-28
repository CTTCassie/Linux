#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_
#include<iostream>
#include<stdlib.h>
#include"jsonApi.h"
class datatype
{
public:
	void val_to_str(std::string &str);
	void str_to_val(std::string &str);
public:
	std::string nick_name;
	std::string school; 
	std::string msg;
	std::string cmd;
};

#endif
