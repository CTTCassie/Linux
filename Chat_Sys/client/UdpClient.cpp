#include"UdpClient.h"

udp_client::udp_client(const std::string ip,const int port)
{
	_server.sin_family=AF_INET;
	_server.sin_port=htons(port);
	_server.sin_addr.s_addr=inet_addr(ip.c_str());
}


void udp_client::initclient()
{
	_sock=socket(AF_INET,SOCK_DGRAM,0);
	if(_sock<0)
	{
		print_log("socket error",FATAL);
		exit(1);
	}
}


int udp_client::recvData(std::string &outString)
{
	//可靠性
	struct sockaddr_in remote;
	socklen_t len=sizeof(remote);
	char buf[SIZE]={0};
	
	ssize_t s=recvfrom(_sock,buf,sizeof(buf),0,(struct sockaddr*)&remote,&len);
	if(s<0)
	{
		print_log("recvfrom error",WARNING);
		return s;
	}
	buf[s-1]='\0';
	//outString=buf;
	int i=0;
	while(buf[i]!='\0')
	{
		outString+=buf[i];
		i++;
	}
	return s;
}


int udp_client::sendData(std::string &inString)
{
	ssize_t s=sendto(_sock,inString.c_str(),inString.size(),0,(struct sockaddr*)&_server,sizeof(_server));
	if(s<0)
	{
		print_log("sendto error",WARNING);
		return s;
	}
	return s;
}



udp_client::~udp_client()
{
	close(_sock);
}
