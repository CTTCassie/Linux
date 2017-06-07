#ifndef __COMM__
#define __COMM__

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

#define PATHNAME "."
#define PROCID 0x6666
#define SIZE 4096*1

int CreatShm();
int GetShm();
//int AtShm();
//int DtShm();
int DestroyShm(int shm_id);

#endif
