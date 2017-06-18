#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void testmyshell()
{
	while(1)
	{
		printf("[ctt@www myshell]@ ");
		fflush(stdout);
		char line[1024];
		ssize_t ret=read(0,line,1024);
		char *myargv[10];
		char *start=line;
		myargv[0]=start;
		int index=1;
		if(ret > 0)      //read correct
		{
			while(*start)  
			{
				if(isspace(*start))  //is space
				{		
					*start='\0';
					start++;
					myargv[index++]=start;
				}
				else
				{
					start++;
				}
			}
		}
		else    //red error
		{
			continue;
		}
		myargv[index-1]=NULL;
		pid_t id=fork();    //creat child process
		if(id == 0) //child
		{
			execvp(myargv[0],myargv);
			perror("fork");
		}  
		else       //parent
		{
			sleep(1);
			wait(NULL);
		}
	}
}

int main()
{
	testmyshell();
	return 0;
}
