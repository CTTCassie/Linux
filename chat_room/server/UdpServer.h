#ifndef _UDP_SERVER_
#define _UDP_SERVER_

#include<iostream>
#include<string>
#include<stdlib.h>
#include<unistd.h>
#include<map>
#include<utility>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"log.h"
#include"DataPool.h"
#include"dataType.h"
#define SIZE 1024

class udp_server
{
public:
	udp_server(const std::string ip,const int port);
	void initServer();
	int recvData(std::string &outString);
	int sendData(std::string &inString,struct sockaddr_in &remote);
	int broadcast();
	~udp_server();
private:
	void addrUser(struct sockaddr_in &remote);
	void delUser(struct sockaddr_in &remote);
private:
	int _sock;
	std::string _ip;
	int _port;
	std::map<in_addr_t,struct sockaddr_in> userList;
	data_pool pool;
};


#endif  
