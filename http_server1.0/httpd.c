#include"httpd.h"

int StartUp(const char *ip,int port)
{
	assert(ip);
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1)
	{
		print_log("socket failed",FATAL);
		exit(2);
	}
	//reuse local_port
	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) == -1)
	{
		print_log("bind failed",FATAL);
		exit(3);
	}
	if(listen(sock,5) == -1)
	{
		print_log("bind failed",FATAL);
		exit(4);
	}
	print_log("create listenfd success",NORMAL);
	return sock;
}

static int get_line(int sock,char *buf,int len)
{
	assert(buf);
	char ch='\0';
	int i=0;

	while(i < len-1 && ch != '\n')
	{
		ssize_t size=recv(sock,&ch,1,0);
		if(size > 0)
		{
			//current is '\r',search the next
			if(ch == '\r')
			{
				if(recv(sock,&ch,1,MSG_PEEK) > 0 && ch == '\n')
					recv(sock,&ch,1,0);
				else
					ch='\n';
			}
			buf[i++]=ch;
		}
		else{
			ch='\n';
		}
	}
	buf[i]='\0';
	return i;
}

static int clear_header(int sock)
{
	int ret=-1;
	char line[SIZE];
	do
	{
		ret=get_line(sock,line,sizeof(line));
	}while(ret != 1 && strcmp(line,"\n") != 0);
	return ret;
}

void print_log(const char *log_msg,int level)
{
	char *arr[10]={
		"NORMAL",
		"WARNING",
		"FATAL",
	};
#define _DEBUG_
#ifdef _DEBUG_
	printf("%s %s\n",log_msg,arr[level]);
	char *path="log/wwwlog";
	int fd=open(path,O_WRONLY|O_APPEND|O_CREAT,0644);

	char buf[SIZE];
	memset(buf,0,SIZE);

	strncpy(buf,log_msg,strlen(log_msg));

	strncat(buf,arr[level],strlen(arr[level]));

	time_t tm;
	char str[30];
	str[0] = ' ';
	time(&tm);
	strcpy(str+1,ctime(&tm));
	strcat(buf,str);
	write(fd,buf,strlen(buf));
#endif
	close(fd);
}

void request_404(int sock)
{
	printf("enter request_404...\n");
	char *path="./wwwroot/404.html";

	struct stat st;
	if(stat(path,&st) < 0){
		return;
	}
	int fd=open(path,O_RDONLY);
	if(fd < 0)
	{
		print_log("open failed",FATAL);
		return;
	}
	char buf[SIZE];
	memset(buf,0,SIZE);

	sprintf(buf,"HTTP/1.0 404 NotFound\r\n\r\n");

	if(send(sock,buf,strlen(buf),0) < 0){
		print_log("send failed!",FATAL);
		return;
	}

	if(sendfile(sock,fd,NULL,sizeof(buf)) < 0){
		print_log("sendfile failed!",FATAL);
		return;
	}

    const char *content_type="Content-Type:text/html;charset=ISO-8859-1\r\n";
    send(sock,content_type,strlen(content_type),0);
    send(sock,"\r\n",strlen("\r\n"),0);

	close(fd);
}

void echo_error(int sock,int err_code)
{
	printf("enter echo_error...\n");
	switch(err_code)
	{
		case 401:
			break;
		case 404:
			request_404(sock);
			break;
		case 500:
			break;
		case 503:
			break;
		default:
			break;
	}
}

static int echo_www(int sock,const char *path,ssize_t size)
{
	int ret=-1;
	int fd=open(path,O_RDONLY);
	if(fd == -1)
	{
		print_log("open failed",FATAL);
		echo_error(sock,404);
		ret=9;
	}
	char line[10*SIZE];
	sprintf(line,"HTTP/1.0 200 OK\r\n");
	send(sock,line,strlen(line),0);

	send(sock,"\r\n",strlen("\r\n"),0);

	if(sendfile(sock,fd,NULL,size) < 0)
	{
		print_log("sendfile failed",FATAL);
		echo_error(sock,404);
		ret=10;
	}
	close(sock);
	return ret;
}

static int excu_cgi(int sock,const char *method,const char *path,const char *query_string)
{
	printf("excu_cgi is running...\n");
	int ret=0;
	int content_len=0;
	if(strcasecmp(method,"GET") == 0){
		clear_header(sock);//read empty line
		printf("clear query_string is %s\n",query_string);
	}
	else//POST
	{
		char buf[SIZE];
		memset(buf,0,sizeof(buf));
		do
		{
			ret=get_line(sock,buf,sizeof(buf));
			if(strncmp(buf,"Content-Length: ",strlen("Content-Length: ")) == 0){
				content_len=atoi(buf+strlen("Content-Length: "));
			}
		}while(ret != 1 && strcmp(buf,"\n") != 0);
	}
	//maybe get content_len
	//GET-->0,POST-->15
	//must clear_header
	if(content_len < 0){
		echo_error(sock,404);
		return 1;
	}

	printf("content_len=%d\n",content_len);
	const char *status_line="HTTP/1.0 200 OK\r\n";
    send(sock,status_line,strlen(status_line),0);
    
    const char *content_type="Content-Type:text/html;charset=ISO-8859-1\r\n";
    send(sock,content_type,strlen(content_type),0);
    send(sock,"\r\n",strlen("\r\n"),0);

	int input[2];
	int output[2];
	char method_env[SIZE/8];
	char query_string_env[SIZE/4];
	char content_len_env[SIZE/8];

	pipe(input);
	pipe(output);

	pid_t fd=fork();
	if(fd < 0)
	{
		perror("fork");
		echo_error(sock,404);
		return 2;
	}
	else if(fd == 0)//child->read
	{
		printf("child is running...\n");
		close(input[1]);  //close write
		close(output[0]); //close read
		//close(sock);

		dup2(input[0],0);  //0->read 
		dup2(output[1],1); //1->write

		sprintf(method_env,"METHOD=%s",method);
		putenv(method_env);

		if(strcasecmp(method,"GET") == 0)
		{
			sprintf(query_string_env,"QUERY_STRING=%s",query_string);
			putenv(query_string_env);
		}
		else//POST
		{
			sprintf(content_len_env,"CONTENT_LEN=%d",content_len);
			putenv(content_len_env);
		}

		execl(path,path,NULL);
		exit(1);
	}
	else //father
	{
		printf("father is running...\n");
		close(input[0]);
		close(output[1]);

		char ch='\0';
		if(strcasecmp(method,"POST") == 0)   //POST,read data from sock
		{
			int i=0;
			for(i=0;i<content_len;i++)
			{
				recv(sock,&ch,1,0);
				write(input[1],&ch,1);
			}
		}
		while(read(output[0],&ch,1) > 0){
			send(sock,&ch,1,0);
		}
		if(waitpid(fd,NULL,0) != -1){
			printf("wait child success...\n");
		}
	}//father
}

int handler_sock(int sock)
{
	char method[64];
	char url[SIZE];
	char buf[SIZE];
	int ret=0;
	int cgi=0;
	int i=0,j=0;
	if(get_line(sock,buf,sizeof(buf)) < 0)
	{
		print_log("get_line",WARNING);
		ret=6;
		goto end;
	}

	printf("get line:%s\n",buf);
	//get method
	while(i<sizeof(buf)-1 && j<sizeof(method)-1 && !isspace(buf[i]))
	{
		method[j]=buf[i];
		j++;
		i++;
	}
	method[j]=0;

	printf("method=%s\n",method);

	//GET or POST
	if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
	{
		print_log("not GET or POST",WARNING);
		echo_error(sock,404);
		ret=7;
		goto end;
	}

	if(strcasecmp(method,"POST") == 0)
	{
		cgi=1;
	}
	//find space,find url,i->/XX/YY/ZZ?a=a&b=b&c=c HTTP/1.0
	while(i<sizeof(buf)-1 && isspace(buf[i]))
	{
		i++;
	}
	j=0;
	while(i<sizeof(buf)-1 && j<sizeof(url)-1 && !isspace(buf[i]))
	{
		url[j]=buf[i];
		j++;
		i++;
	}
	url[j]=0;
	printf("url=%s\n",url);

	char *query_string=NULL;
	if(strcasecmp(method,"GET") == 0)
	{
		query_string=url;
		printf("query_string=%s\n",query_string);
		while(*query_string != '?' && *query_string != '\0')
		{
			query_string++;
		}
		//path?data
		if(*query_string == '?')
		{
			*query_string='\0';
			query_string++;
			cgi=1;
		}
	}
	printf("query_string=%s\n",query_string);
	//1.GET or POST
	//2.cgi-->OK
	//3.GET-->query_string,POST-->recv
	//4.url
	char path[SIZE];
	memset(path,0,SIZE);
	sprintf(path,"wwwroot%s",url);
	if(path[strlen(path)-1] == '/'){
		strcat(path,"index.html");
	}

    printf("path=%s\n",path);
	struct stat st;
	if(stat(path,&st) == -1){
		print_log("stat failed",WARNING);
		echo_error(sock,404);
	    ret=8;
	    goto end;
	}
	else   //file exist
	{
		printf("file exist...\n");
		if(S_ISDIR(st.st_mode))   //document,strcat index.html
		{
			strcat(path,"/index.html");
		}
		else if(S_ISREG(st.st_mode)) //common file
		{
			printf("file is normal!\n");
			if((st.st_mode & S_IXUSR) || \
			   (st.st_mode & S_IXGRP) || \
			   (st.st_mode & S_IXOTH))   // judge executable authority    
			{
				cgi=1;
			}
			printf("cgi=%d\n",cgi);
		}
	}
	//cgi=0-->GET and no data 
	//cgi=1-->POST or GET have data
	if(cgi){
		excu_cgi(sock,method,path,query_string);
	}
	else//not cgi
	{
		clear_header(sock);
		echo_www(sock,path,st.st_size);
	}
end:
	close(sock);
	return ret;
}


