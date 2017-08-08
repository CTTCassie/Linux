#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

//#include"BigData.h" 
#define _SIZE_ 4096
//data1=100&data2=200
static void math_data(char *data)
{
	printf("cgi is running!\n");
	char *str[3];
	char *ptr=data;
	int i=0;
	while(*ptr)
	{
		if(*ptr == '='){
			str[i]=ptr+1;
			i++;
		}
		else if(*ptr == '&'){
			*ptr='\0';
		}
		else{
		}
		ptr++;
	}
	str[i]=NULL;
	int data1=atoi(str[0]);
    int data2=atoi(str[1]);
   // BigData data1(str[0]);
   // BigData data2(str[1]);
	printf("<html>");
	printf("<body>");

	printf("<h2>%d + %d=%d</h2><br/>",data1,data2,data1+data2);
	printf("<h2>%d - %d=%d</h2><br/>",data1,data2,data1-data2);
	printf("<h2>%d * %d=%d</h2><br/>",data1,data2,data1*data2);
	printf("<h2>%d / %d=%d</h2><br/>",data1,data2,data1/data2);
	printf("<h2>%d %% %d=%d</h2><br/>",data1,data2,data1%data2);
	printf("</body>");
	printf("</html>");
}

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
	
	printf("cgi method=%s\n",method);

	if(strcasecmp(method,"GET") == 0){
		printf("QUERY_STRING...\n");
		if(getenv("QUERY_STRING")){
			strcpy(content_data,getenv("QUERY_STRING"));
		}
		else{
			perror("getenv query_string error");
			return 2;
		}
	}
	else{ //POST
		printf("CONTENT_LEN...\n");
		if(getenv("CONTENT_LEN")){
			strcpy(content_len,getenv("CONTENT_LEN"));
			int len=atoi(content_len);
			int i=0;
			char ch='\0';
			for(i=0;i<len;i++){
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
	math_data(content_data);
	return 0;
}
