#include"UdpServer.h"
udp_server::udp_server(const std::string ip,const int port)
	:_ip(ip)
	,_port(port)
	,pool(1024)
{}


void udp_server::initServer()
{
	_sock=socket(AF_INET,SOCK_DGRAM,0);
	if(_sock<0)
	{
		print_log("socket error",FATAL);
		exit(1);
	}
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(_port);
	local.sin_addr.s_addr=inet_addr(_ip.c_str());
	if(bind(_sock,(struct sockaddr *)&local,sizeof(local))<0)
	{
		print_log("socket error",FATAL);
		exit(2);
	}
}


void udp_server::addrUser(struct sockaddr_in &remote)
{
	userList.insert(std::pair<in_addr_t,struct sockaddr_in>(remote.sin_addr.s_addr,remote));
}


void udp_server::delUser(struct sockaddr_in &remote)
{
	std::map<in_addr_t,struct sockaddr_in>::iterator iter=userList.find(remote.sin_addr.s_addr);
	if(iter!=userList.end())
		userList.erase(iter);
}


int udp_server::recvData(std::string &outString)
{
	//可靠性
	struct sockaddr_in remote;
	socklen_t len=sizeof(remote);
	char buf[SIZE];
	ssize_t s=recvfrom(_sock,buf,sizeof(buf),0,(struct sockaddr*)&remote,&len);
	if(s<0)
	{
		print_log("recvfrom error",WARNING);
		return s;
	}
	buf[s]='\0';
	outString=buf;
	pool.putData(outString);
	
	datatype data;
	data.str_to_val(outString);

	if(data.cmd=="QUIT")
	{
		delUser(remote);	//有用户退出，从用户列表中删除
	}
	else
	{
		addrUser(remote);
	}
	return s;
}


int udp_server::sendData(std::string &inString,struct sockaddr_in &remote)
{
	ssize_t s=sendto(_sock,inString.c_str(),inString.size(),0,(struct sockaddr*)&remote,sizeof(remote));
	if(s<0)
	{
		print_log("sendto error",WARNING);
		return s;
	}
	return s;
}


int udp_server::broadcast()
{
	std::string inString;
	pool.getData(inString);
	std::map<in_addr_t,struct sockaddr_in>::iterator iter=userList.begin();
	for(;iter!=userList.end();iter++)
	{
		sendData(inString,iter->second);
	}
}

udp_server::~udp_server()
{
	close(_sock);
}
