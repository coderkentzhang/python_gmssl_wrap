
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ssock_wrap.h"

using namespace py_ssock_wrap;

int main(int argc, char **argv)
{   
	printf("wrap test start->\n");
	SSockWrap sw;
	char buff[256]="wrap test  aaccbb";
	int retval = 0;   
	retval = sw.init();
	printf("init ret %d\n",retval);
	retval = sw.write(buff,strlen(buff));

	retval = sw.read(buff,sizeof(buff));
	printf("!after read %s\n",buff);
	sw.shutdown();
	sw.close();
	return 0;	
} 

