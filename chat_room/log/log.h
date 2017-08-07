#ifndef _LOG_H_
#define _LOG_H_
#include<stdio.h>
#include<iostream>
#define NORMAL  0
#define NOTICE  1
#define WARNING 2
#define FATAL   4


void print_log(std::string msg,int l);
#endif
