#include"sql_api.h"

sql::sql()
{
	mysql_init(&m_sql);
}

sql::~sql()
{
	mysql_close(&m_sql);
}

int sql::sql_connect()
{
	bool ret=true;
	if(mysql_real_connect(&m_sql,"127.0.0.1","root","ctt","test",3306,NULL,0) == NULL)
	{
		cout<<"connect failed..."<<endl;
		ret=false;
		goto end;
	}
	cout<<"connect success..."<<endl;
end:
	return ret;
}

int sql::sql_insert(const std::string& data)
{
	string str="INSERT INTO student_info(name,sex,age,school)VALUES(";
	str += data;
	str += ")";
	mysql_query(&m_sql,str.c_str());
}

void sql::sql_select(const std::string& data)
{
	cout<<"sql_select:"<<data.c_str()<<endl;
	//data-->'ctt','woman',21,'xatu'
	string str="SELECT * FROM student_info";
	if(mysql_query(&m_sql,str.c_str()) == 0)
	{
		cout<<"enter..."<<endl;
		MYSQL_RES *res=mysql_store_result(&m_sql);
		int rows=mysql_num_rows(res);
		int fields=mysql_num_fields(res);
		cout<<"rows:"<<rows<<endl;
		cout<<"fieds:"<<fields<<endl;

		MYSQL_ROW line;
		for(int i=0;i<rows;i++)
		{
			line=mysql_fetch_row(res);
			for(int j=0;j<fields-1;j++)
			{
				cout<<line[j]<<" ";
			}
			cout<<endl;
		}
	}
}

void sql::sql_delete(const std::string& data)
{
	string str="DELETE FROM student_info WHERE name=";
	str += data;
	str += ";";
	mysql_query(&m_sql,str.c_str());
}

void sql::sql_update(const std::string& data)
{
	string str="UPDATE student_info SET school='xatu' WHERE name=";
	str += data;
	str += ";";
	mysql_query(&m_sql,str.c_str());
}
