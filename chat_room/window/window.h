#ifndef _MY_WINDOW_H_
#define _MY_WINDOW_H_
#include<iostream>
#include<stdlib.h>
#include<ncurses.h>
#include<string>

class myWindow
{
public:
	myWindow();
	~myWindow();
	void createHeader(); //绘制标题区
	void createOutput(); //绘制输出区
	void createFriendList(); //绘制好友列表区
	void createInput(); //绘制输出区
	void myWinRefresh(WINDOW *win); //刷新窗口
	WINDOW* createNewWin(int h,int w,int startY,int startX); //绘制窗口
	void putStrToWin(WINDOW *win,int startY,int startX,const std::string &msg); //把消息放到窗口中
	void clearWinLine(WINDOW* win,int begin,int line); //清空窗口
	std::string getWinStr(WINDOW* win); //获得窗口里面的消息 
public:
    //界面分为四个区域
	WINDOW *header; //标题区
	WINDOW *output; //输出区
	WINDOW *friendList; //好友列表
	WINDOW *input; //输入区
};

#endif
