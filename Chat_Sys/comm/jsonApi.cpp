#include"jsonApi.h"

json_api::json_api()
{}


void json_api::serialize(Json::Value &val,std::string &str)
{
#ifdef _FAST_
	Json::FastWriter w;
	str=w.write(val);
#else
	Json::StyledWriter w;
	str=w.write(val);
#endif
}


void json_api::deserialize(std::string &str,Json::Value &val)
{
	Json::Reader r;
	r.parse(str,val,false);
}

json_api::~json_api()
{}
