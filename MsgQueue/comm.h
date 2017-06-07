#ifndef __COMM__
#define __COMM__

#define PATHNAME "."
#define PROCID 0x6666
#define MYSIZE 256
#define CLIENT_TYPE 1
#define SERVER_TYPE 2

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<unistd.h>

struct msgbuf
{
	long msg_type;
	char text[MYSIZE];
};

int CreatMsgQueue();
int DestroyMsgQueue(int msg_id);
int GetMsgQueue();
int SendMsgQueue(int msg_id,long type,const char *msg);
int ReceiveMsgQueue(int msg_id,long type,char out[]);

#endif
