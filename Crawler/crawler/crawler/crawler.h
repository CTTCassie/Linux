#pragma once
#include<iostream>
using namespace std;
#include"log.h"
#include<iconv.h>
#include<curl/curl.h>
#include<boost/regex.hpp>
#include<errno.h>
#include<vector>
#include<string>
#include<fstream>

#define SIZE 1024*100

bool GetHtml(const string& url,const string *html);  //获取指定url的html代码
bool GetUrlList(const string& html,vector<string> *urlList);  //从首页的html中获取url列表
bool GetTitle(const string& html,string *title);  //获取标题
bool Save(const string& path,int num,const string& file,const string& html);
int CodeConvert(const char *from,const char *to,char *inbuf,size_t inLen,char *outbuf,size_t outLen);
void Run();   //定义爬虫的入口函数
