#ifndef _JSON_API_
#define _JSON_API_
#include<iostream>
#include<string>
#include<json/json.h>

class json_api
{
public:
	json_api();
    static void serialize(Json::Value &val,std::string &str); //序列化-->多个数据转成一个字符串
	static void deserialize(std::string &str,Json::Value &val); //反序列化-->将一个字符串拆分成若干个数据
	~json_api();
};

#endif
