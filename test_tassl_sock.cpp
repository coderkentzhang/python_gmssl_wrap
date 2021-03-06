#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include "tassl_sock_wrap.h"


using namespace fisco_tassl_sock_wrap;

void printhex(char *str, int len)
{
	char buff[4096]="\0";
	int i;
	for(i=0;i<len;i++)
	{
		char s[10]="\0";
		sprintf(s,"%02x",(char)str[i]);
		strcat(buff,s);
	}
	printf("%s\n",buff);
}

/*
length	uint32_t	4	数据包长度，含包头和数据，大端
type	uint16_t	2	数据包类型，大端
seq	string	32	数据包序列号，32字节uuid
result	int32_t	4	错误码，大端
data	bytes	length-42	数据包体，字节流
*/
int make_channel_pack(char *data,int len,char *output)
{
		short type = 0x12;
		int headerlen = (4+2+32+4);
		int totallen = headerlen + len;
		int net_totallen = htonl(totallen);
		int pos = 0;
		memcpy(output+pos,&net_totallen,4);
		pos+=4;
		type = htons(type);
		memcpy(output+pos,&type,2);
		pos+=2;
		char seqid[256]="12345678901234567890123456789012";
		memcpy(output+pos,&seqid,32);
		pos+=32;
		int code =0;
		memcpy(output+pos,&code,4);
		pos+=4;
		memcpy(output+pos,data,len);
		pos+=len;
		printf("encode done pos = %d , datalen = %d\n",pos,len);
		printhex(output,totallen);
		int lll;
		memcpy(&lll,output,4);
		printf("check length in header : %d\n",ntohl(lll));
 
		return totallen;
}



int main(int argc, char **argv)
{   
	printf("wrap test start->\n");
	TasslSockWrap wrap;
	char buff[4096]="tassl wrap test";
	int retval = 0;   
	printf("1>");
	wrap.set_host_port("127.0.0.1",20800);
	printf("2>");
	wrap.set_ca_files("gmca.crt","gmsdk.crt","gmsdk.key","gmensdk.crt","gmensdk.key");
	printf("3>\n");
	retval = wrap.init();
	printf("init ret %d\n",retval);
	retval = wrap.try_connect();
	
	char reqtext[1024] = "{\"jsonrpc\":\"2.0\",\"method\":\"getClientVersion\",\"params\":[],\"id\":1}";
	char channelpack[4096] =  "\0";
	int packlen = make_channel_pack(reqtext,strlen(reqtext),channelpack);
	retval = wrap.write(channelpack,packlen);
	retval = wrap.read(buff,sizeof(buff));
	printf("!after read ret len: %d\n",retval);
	char data[4096]="\0";
	strncpy(data,buff+42,retval-42);
	printf("server return :%s\n",data);
	wrap.finish();
	return 0;	
} 

