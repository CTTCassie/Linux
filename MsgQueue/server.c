#include"comm.h"

int main()
{
	int msg_id=CreatMsgQueue();
	char buf[2*MYSIZE];
	while(1)
	{
		if(ReceiveMsgQueue(msg_id,CLIENT_TYPE,buf) < 0)
		{
			break;
		}
		printf("client# %s\n",buf);
		if(SendMsgQueue(msg_id,SERVER_TYPE,buf) < 0)
		{
			break;
		}
	}
	DestroyMsgQueue(msg_id);
	return 0;
}
