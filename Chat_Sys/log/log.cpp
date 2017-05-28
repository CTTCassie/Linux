#include"log.h"

std::string lev[]={"[normal] ","[notice] ","[warning] ","[fatal] "};
void print_log(std::string msg,int l)
{
#ifdef DEBUG
	if(l>=0&&l<4)
		printf("%s %s\n",msg.c_str(),lev[l].c_str());
#endif
}
