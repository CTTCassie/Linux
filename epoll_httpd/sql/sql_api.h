#ifndef __MYSQL_H__
#define __MYSQL_H__

#include<iostream>
using namespace std;

#include<stdio.h>
#include<mysql.h>
#include<string>

class sql
{
public:
	sql();
	~sql();
	int sql_connect();
	int sql_insert(const std::string& data);
	void sql_select(const std::string& data);
	void sql_delete(const std::string& data);
	void sql_update(const std::string& data);
private:
	MYSQL m_sql;
};


#endif
