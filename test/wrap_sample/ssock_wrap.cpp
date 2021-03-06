#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "ssock_wrap.h"

using namespace py_ssock_wrap;
 
SSockWrap::SSockWrap(void)
{
	 
}

SSockWrap::~SSockWrap(void)
{
	
}


int SSockWrap::init()
{
	int retval = 0; 
	return retval;
}


int SSockWrap::shutdown()
{
	printf("SSockWrap::shutdown!!\n");
	return 0;
}

int SSockWrap::close()
{
	printf("SSockWrap::close.\n");
	return 0;
}

int SSockWrap::write(const char * buffer,const int len)
{
	printf("[ in cpp ]:write len %d : %s\n",len,buffer);
	return 0;	
}

int SSockWrap::read(char *buffer, const int buffersize)
{
	snprintf(buffer,buffersize,"[ in cpp ] >>on read ,buffersize %d ",buffersize);
	return buffersize;
}
