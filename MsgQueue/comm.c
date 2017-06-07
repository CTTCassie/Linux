#include"comm.h"

static int ComMsgQueue(int flag)
{
	key_t key=ftok(PATHNAME,PROCID);
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}
	int msg_id=msgget(key,flag);
	if(msg_id < 0)
	{
		perror("msgget");
		return -2;
	}
	return msg_id;
}

int CreatMsgQueue()
{
	return ComMsgQueue(IPC_CREAT|IPC_EXCL|0x666);
}

int GetMsgQueue()
{
	return ComMsgQueue(IPC_CREAT);
}

int SendMsgQueue(int msg_id,long type,const char *msg)
{
	struct msgbuf buf;
	buf.msg_type=type;
	strcpy(buf.text,msg);
	int ret=msgsnd(msg_id,&buf,sizeof(buf.text),0);
	if(ret < 0)
	{
		perror("msgsnd");
		return -1;
	}
	return 0;
}

int ReceiveMsgQueue(int msg_id,long type,char out[])
{
	struct msgbuf buf;
	int size=msgrcv(msg_id,&buf,sizeof(buf.text),type,0);
	if(size > 0)
	{
		buf.text[size]='\0';
		strncpy(out,buf.text,size);
		return 0;
	}
	perror("msgrcv");
	return -1;
}
int DestroyMsgQueue(int msg_id)
{
	if(msgctl(msg_id,IPC_RMID,NULL) < 0)
	{
		perror("msgctl");
		return -1;
	}
	return 0;
}
