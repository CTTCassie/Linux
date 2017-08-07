#include"dataType.h"

datatype::datatype()
{}

datatype::~datatype()
{}

void datatype::val_to_str(std::string &str) //序列化
{
	Json::Value val;
	val["nick_name"]=nick_name;
	val["school"]=school;
	val["msg"]=msg;
	val["cmd"]=cmd;
	json_api::serialize(val,str);
}

void datatype::str_to_val(std::string &str) //反序列化
{
	Json::Value val;
	json_api::deserialize(str,val);
    //asSting()提取序列化之后的单个信息
	nick_name=val["nick_name"].asString();
	school=val["school"].asString();
	msg=val["msg"].asString();
	cmd=val["cmd"].asString();
}

#ifdef _DEBUG_
int main()
{
    datatype d;
    d.nick_name="ONEDAY";
    d.school="xatu";
    d.msg="hello Json";
    d.cmd="";
    std::string str;
    d.val_to_str(str);
    std::cout<<str<<std::endl;
}
#endif //_DEBUG_
