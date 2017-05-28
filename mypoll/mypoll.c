#include<stdio.h>
#include<poll.h>

int main()
{
	struct pollfd in_fds;
	in_fds.fd=0;
	in_fds.events=POLLIN;
	in_fds.revents=0;
	int timeout=5000;
	while(1)
	{
		switch(poll(&in_fds,1,timeout))
		{
			case -1:    //error
				perror("poll");
				break;
			case 0:    //timeout
				printf("timeout...\n");
				break;
			default:  
				{
					char buf[256];
					if((in_fds.revents != 0) && (in_fds.revents & POLLIN))
					{
						//read is ready and happen POLLIN event
						ssize_t size=read(in_fds.fd,buf,sizeof(buf)-1);
						if(size > 0)
						{
							buf[size-1]='\0';
							printf("client# %s\n",buf);
						}
						else if(size == 0)
						{
							printf("read ending\n");
							return 1;
						}
						else
						{
							perror("read");
							return 2;
						}
					}
				}
				break;
		}
	}
	return 0;
}
