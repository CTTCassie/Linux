#ifndef _JSON_API_
#define _JSON_API_
#include<iostream>
#include<json/json.h>

class json_api
{
public:
	json_api();
    static void serialize(Json::Value &val,std::string &str);
	static void deserialize(std::string &str,Json::Value &val);
	~json_api();
};

#endif
