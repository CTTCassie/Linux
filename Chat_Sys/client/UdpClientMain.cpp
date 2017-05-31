#include"UdpClient.h"
#include"dataType.h"
#include"window.h"
#include<signal.h>
#include<set>
using namespace std;

void Usage(const char *usage)
{
	printf("Usage: %s [remote_ip] [remote_port] [nick_name] [school]\n",usage);
}

myWindow win;
datatype senddata;
datatype recvdata;
set<string> circle_friend;
udp_client *_client=NULL;


void* header_run(void* arg)   //实现类似循环滚动的效果
{	
	int y,x;
	std::string msg="Welcome to chat_sys!";
	int step=0;

	while(1)
	{	
		win.createHeader();
		win.myWinRefresh(win.header);
		getmaxyx(win.header,y,x);
		int startY=y*4/10;
		int startX=x;
		
		win.putStrToWin(win.header,startY,step++,msg);
		win.myWinRefresh(win.header);
		usleep(200000);
	
		step%=(startX-1);

		win.clearWinLine(win.header,startY,1);
		win.myWinRefresh(win.header);
	}
	return NULL;
}


void* output_run(void* arg)   //当output满的时候可自动刷新，重新接受新的数据
{
	udp_client *client=(udp_client*)arg;
	int step=1;
	int y=0;
	int x=0;	
	win.createOutput();
	win.myWinRefresh(win.output);
	while(1)
	{	
		std::string msg;
		client->recvData(msg);
         //反序列化	recvdata
		recvdata.str_to_val(msg);
		msg.clear();

		std::string name_school=recvdata.nick_name+"-"+recvdata.school;
		msg=name_school+": "+recvdata.msg;

		if(recvdata.cmd=="QUIT")
		{
			circle_friend.erase(name_school);
		}
		else
		{
			circle_friend.insert(name_school);
	
			win.putStrToWin(win.output,step++,1,msg);
			win.myWinRefresh(win.output);
			getmaxyx(win.output,y,x);
			int startY=y;	
			step%=startY;
			if(step==0)
			{
				win.createOutput();
				win.myWinRefresh(win.output);
				step=1;	
				win.putStrToWin(win.output,step++,1,msg);
				win.myWinRefresh(win.output);
			}
		}  //if
	}      //while
	return NULL;
}


void* friendList_run(void* arg)
{
 	win.createFriendList();
 	win.myWinRefresh(win.friendList);
	int x,y;
	int step=1;
	while(1)
	{	
		std::string msg;
        set<string>::iterator iter=circle_friend.begin();

		win.createFriendList();
		win.myWinRefresh(win.friendList);
		step=1;
		for(;iter!=circle_friend.end();iter++)
		{
            msg=*iter;
			win.putStrToWin(win.friendList,step++,1,msg);
			win.myWinRefresh(win.friendList);
	
			getmaxyx(win.friendList,y,x);
			int startY=y;	
			step%=startY;
			if(step==0)
			{
				sleep(3); 
				win.createFriendList();
				win.myWinRefresh(win.friendList);
				step=1;	
				win.putStrToWin(win.friendList,step++,1,msg);
				win.myWinRefresh(win.friendList);
			}
		}  //for
		sleep(1);
	}   //while	
	return NULL;
}

void* input_run(void* arg)
{
	udp_client *client=(udp_client*)arg;
	std::string point="Please Enter: ";				
	while(1)
	{
		win.createInput();
		win.myWinRefresh(win.input);
		
		win.putStrToWin(win.input,1,1,point);
		win.myWinRefresh(win.input);
	
		std::string msg=win.getWinStr(win.input);//获取窗口数据
		senddata.msg=msg;
		
		senddata.val_to_str(msg);//序列化
	    client->sendData(msg);	
	}
	return NULL;
}

void  hander(int sig)
{
	std::string msg;
	senddata.cmd="QUIT";
	senddata.msg="None";
	senddata.val_to_str(msg);
	_client->sendData(msg);
	exit(1);
}


int main(int argc,char* argv[])
{
	if(argc!=5)
	{
		Usage(argv[0]);
	}

	senddata.nick_name=argv[3];
	senddata.school=argv[4];
	senddata.cmd="None";

	string ip=argv[1];
	int port=atoi(argv[2]);
	udp_client client(ip,port);
	client.initclient();
	_client=&client;

	struct sigaction sigSet;
	sigSet.sa_handler=hander;
	sigemptyset(&sigSet.sa_mask);
	sigSet.sa_flags=0;
	sigaction(2,&sigSet,NULL);

	pthread_t headerID; 
	pthread_t outputID; 
	pthread_t friendListID; 
	pthread_t inputID; 

	pthread_create(&headerID,NULL,header_run,NULL);
	pthread_create(&outputID,NULL,output_run,(void*)&client);
	pthread_create(&friendListID,NULL,friendList_run,NULL);
	pthread_create(&inputID,NULL,input_run,(void*)&client);

	pthread_join(headerID,NULL);
	pthread_join(outputID,NULL);
	pthread_join(friendListID,NULL);
	pthread_join(inputID,NULL);

	return 0;
}
