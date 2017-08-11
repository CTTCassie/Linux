#include"sql_api.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<strings.h>
#include<string>
#define _SIZE_ 4096

int main()
{
	char method[_SIZE_];
	char content_data[_SIZE_];
	char content_len[_SIZE_];
	if(getenv("METHOD")){
		strcpy(method,getenv("METHOD"));
	}
	else{
		perror("getenv method error");
		return 1;
	}

	cout<<"method="<<method<<endl;

	if(strcasecmp(method,"GET") == 0){
		if(getenv("QUERY_STRING")){
			strcpy(content_data,getenv("QUERY_STRING"));
		}
		else{
			perror("getenv query_string error");
			return 2;
		}
	}
	else{ //POST
		if(getenv("CONTENT_LEN")){
			strcpy(content_len,getenv("CONTENT_LEN"));
			int len=atoi(content_len);
			int i=0;
			char ch='\0';
			for(int i=0;i<len;i++)
			{
				read(0,&ch,1);
				content_data[i]=ch;
			}
			content_data[i]='\0';
		}
		else{
			perror("getenv content_len error");
			return 3;
		}
	}//else

	char *arr[5];
	int i=0;
	char *str=content_data;
	//name=ctt&sex=woman&age=21&school=xatu
	while(*str)
	{
		if(*str == '='){
			arr[i]=str+1;
			i++;
		}
		else if(*str == '&'){
			*str='\0';
		}
		else{
		}
		str++;
	}
	arr[i]=NULL;

	//get data from browser

	string data;
	int n=5;
	for(int j=0;j<n;j++)
	{
		if(j == 2)
		{
			data += arr[j];
			data += ',';
			continue;
		}
		if(j == n-1)
			break;
		data += "\'";
		data += arr[j];
		data += "\'";
		if(j < n-2){
			data += ",";
		}
	}

	sql s;
	s.sql_connect();
	s.sql_insert(data);
	return 0;
}
