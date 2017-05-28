#ifndef _UDP_client_
#define _UDP_client_
#include<unistd.h>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"log.h"
#define SIZE 1024

class udp_client
{
public:
	udp_client(const std::string ip,const int port);
	void initclient();
	int recvData(std::string &outString);
	int sendData(std::string &inString);
	~udp_client();
private:
	int _sock;
	struct sockaddr_in _server;
};


#endif  
