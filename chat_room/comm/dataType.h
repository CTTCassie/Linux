#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_
#include<iostream>
#include<stdlib.h>
#include"jsonApi.h"

class datatype
{
public:
    datatype();
	void val_to_str(std::string &str); //序列化这四个成员对象
	void str_to_val(std::string &str); //反序列化
    ~datatype();
public:
	std::string nick_name;//昵称
	std::string school;   //学校
	std::string msg;      //发送的消息正文
	std::string cmd;      //退出信息
};

#endif
