#include"jsonApi.h"

json_api::json_api()
{}

void json_api::serialize(Json::Value &val,std::string &str) //序列化
{
#ifdef _FAST_
	Json::FastWriter w;
	str=w.write(val);  //序列化的方法
#else
	Json::StyledWriter w;
	str=w.write(val);
#endif
}

void json_api::deserialize(std::string &str,Json::Value &val) //反序列化
{
	Json::Reader r;  //反序列化的方法
	r.parse(str,val,false);
}

json_api::~json_api()
{}
