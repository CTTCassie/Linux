#ifndef _COMM_H_
#define _COMM_H_
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/wait.h>

#define PATHNAME "."
#define PROJID 0x6666

union semun{
	int val;                 /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
};

int CreateSemSet(int num);//创建信号量
int GetSemSet();       //获取信号量
int InitSem(int sem_id,int which);
int P(int sem_id,int which); //p操作
int V(int sem_id,int which); //v操作
int DestroySemSet(int sem_id);//销毁信号量

#endif //_COMM_H_
