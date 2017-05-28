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
	void createHeader();
	void createOutput();
	void createFriendList();
	void createInput();
	void myWinRefresh(WINDOW *win);
	WINDOW* createNewWin(int h,int w,int startY,int startX);
	void putStrToWin(WINDOW *win,int startY,int startX,const std::string &msg);
	void clearWinLine(WINDOW* win,int begin,int line);
	std::string getWinStr(WINDOW* win);
public:
	WINDOW *header;
	WINDOW *output;
	WINDOW *friendList;
	WINDOW *input;
};

#endif
