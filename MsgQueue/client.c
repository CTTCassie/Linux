#include"comm.h"

int main()
{
	int msg_id=GetMsgQueue();
	char buf[MYSIZE];
	char out[2*MYSIZE];
	while(1)
	{
		printf("please enter#");
		fflush(stdout);
		ssize_t size=read(0,buf,sizeof(buf)-1);
		if(size > 0)
		{
			buf[size-1]='\0';
			SendMsgQueue(msg_id,CLIENT_TYPE,buf);
		}
		if(ReceiveMsgQueue(msg_id,SERVER_TYPE,out) < 0)
		{
			break;
		}
		printf("server echo#%s\n",out);
	}
	return 0;
}
