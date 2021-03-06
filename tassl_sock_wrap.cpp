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



void print_ssl_error(int err,const char *memo)
{
	char err_type[1024]="\0";
	switch(err)
	{
		
		case SSL_ERROR_NONE  :   
			strcpy(err_type,"SSL_ERROR_NONE");
			break;
		case SSL_ERROR_SSL   :    
			strcpy(err_type,"SSL_ERROR_SSL"); 
			break;
		case SSL_ERROR_WANT_READ   : 
			strcpy(err_type,"SSL_ERROR_WANT_READ"); 
			break;		
		case SSL_ERROR_WANT_WRITE  : 
			strcpy(err_type,"SSL_ERROR_WANT_WRITE"); 
			break;		
		case SSL_ERROR_WANT_X509_LOOKUP :
			strcpy(err_type,"SSL_ERROR_WANT_X509_LOOKUP"); 
			break;		
		case SSL_ERROR_SYSCALL       : 
			strcpy(err_type,"SSL_ERROR_SYSCALL"); 
			break;				                            
		case SSL_ERROR_ZERO_RETURN   : 
			strcpy(err_type,"SSL_ERROR_ZERO_RETURN"); 
			break;		
		case SSL_ERROR_WANT_CONNECT :  
			strcpy(err_type,"SSL_ERROR_WANT_CONNECT"); 
			break;		
		case SSL_ERROR_WANT_ACCEPT  :     
			strcpy(err_type,"SSL_ERROR_WANT_ACCEPT"); 
			break;
		default:
			strcpy(err_type,"unknow");
	}
	printf("[in cpp wrap -->] %s: SSL ERROR : %d -> %s\n",memo,err,err_type);
}


//-------------------------------------------------
 
TasslSockWrap::TasslSockWrap()
{
	 
}

TasslSockWrap::~TasslSockWrap()
{
	
}

int  init_openssl()
{
    int retval = SSL_library_init();
    if (!retval)
	{
		
		return -1;
	}
	//载入所有SSL算法
	OpenSSL_add_ssl_algorithms ();

	//载入所有错误信息
    SSL_load_error_strings();
	ERR_load_crypto_strings();
	return 0;
}


int TasslSockWrap::load_ca_files()
{
	
	int retval;
	
	//加载ca证书
	retval = SSL_CTX_load_verify_locations(ctx,ca_crt_file,NULL); 
	printf("[in cpp wrap -->] SSL_CTX_load_verify_locations %d , %s\n",retval,ca_crt_file);
	if(retval <= 0 )
	{
		printf("[in cpp wrap -->] SSL_CTX_load_verify_locations ERROR ");
		return -101;
	}
	
	
	//加载sdk签名证书
	
	retval = SSL_CTX_use_certificate_chain_file(ctx, sign_crt_file);
	//retval = SSL_CTX_use_certificate_file(ctx, CLIENT_S_CERT,SSL_FILETYPE_PEM);
	printf("[in cpp wrap -->] SSL_CTX_use_certificate_chain_file res =  %d,file: %s\n",retval,sign_crt_file);
	if ( retval<= 0)
	{
		ERR_print_errors_fp(stderr);
		return -102;
	}
	
    //加载sdk key
	retval = SSL_CTX_use_PrivateKey_file(ctx, sign_key_file, SSL_FILETYPE_PEM);
	printf("[in cpp wrap -->] SSL_CTX_use_PrivateKey_file res =  %d,file: %s\n",retval,sign_key_file);
	if ( retval <= 0)
	{
		ERR_print_errors_fp(stderr);
		return -103;
	}
	
	
	//加载sdk en加密证书 
	 
	retval = SSL_CTX_use_certificate_file(ctx, en_crt_file, SSL_FILETYPE_PEM);
	printf("[in cpp wrap -->]SSL_CTX_use_certificate_file res =  %d,file: %s\n",retval,en_crt_file);
	if (retval <= 0)
	{
		ERR_print_errors_fp(stderr);
		return -104;
	}
	
	//加载sdk en加密key
	retval = SSL_CTX_use_enc_PrivateKey_file(ctx, en_key_file, SSL_FILETYPE_PEM);
	printf("[in cpp wrap -->] SSL_CTX_use_enc_PrivateKey_file res =  %d,file: %s\n",retval,en_key_file);
	if (retval <= 0)
	{
		ERR_print_errors_fp(stderr);
		return -105;
	}
		
	//如果客户端开启SSL_CTX_set_verify，则代表客户端需要严格验证服务端的身份，所以客户端需要加载ca来验证对端。服务器端是默认开启。
	SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,NULL);
	SSL_CTX_set_verify_depth(ctx,10); 
		
	//检查证书和key
	if (!SSL_CTX_check_private_key(ctx))
	{
		printf("[in cpp wrap -->] Private key does not match the certificate public key/n");
		return -106;
	}

	
	//检查en证书和key
	if (!SSL_CTX_check_enc_private_key(ctx))
	{
		printf("[in cpp wrap -->] Private key does not match the certificate public key/n");
		return -107;
	}
	
	return 0;
	
}

void TasslSockWrap::set_host_port(const char *host_, int port_)
{
	strcpy(host,host_);
	port = port_;
}
void TasslSockWrap::set_ca_files(const char *ca_crt_file_,
				const char * sign_crt_file_,
				const char * sign_key_file_,
				const char * en_crt_file_,
				const char * en_key_file_
					)
{
	strcpy(ca_crt_file,ca_crt_file_);
	strcpy(sign_crt_file,sign_crt_file_);
	strcpy(sign_key_file,sign_key_file_);
	strcpy(en_crt_file,en_crt_file_);
	strcpy(en_key_file,en_key_file_);								
}

int TasslSockWrap::init()
{
	int retval = 0; 
	printf("[in cpp wrap -->] TasslSockWrap init\n");
	ctx = NULL;
	ssl = NULL;
	retval = init_openssl()	;
	if (retval < 0 )
	{
		return retval;
	}
	ctx = SSL_CTX_new(TLSv1_2_client_method());
	if(ctx == NULL)
	{
		printf("[in cpp wrap -->] Error of Create SSL CTX!\n");
		ERR_print_errors_fp(stderr);
		return -2;
	}
	
	SSL_CTX_set_timeout (ctx, 30); //in seconds 
	SSL_CTX_set_mode (ctx, SSL_MODE_AUTO_RETRY);
	
	retval = load_ca_files();
	ssl = SSL_new (ctx);
	//printf("[in cpp wrap -->] after init ctx %d, ssl %d\n",ctx,ssl);
	return retval;
	
}



int TasslSockWrap::finish()
{
	printf("[in cpp wrap -->] TasslSockWrap::finish.\n");
	if (ssl) {
		SSL_free(ssl);
		ssl = NULL;
	}
	if (ctx) {
		SSL_CTX_free(ctx);
		ctx = NULL;
	}
	return 0;
}

int TasslSockWrap::try_connect()
{
	int sock;
	int retval;
	int err;
	printf("\n[in cpp wrap -->] ----- use SOCKET mode to connect server : %s, %d\n\n",host,port);
	/* 创建socket描述符 */
	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf ("SOCKET error. \n");
		return -201;
	}
	struct sockaddr_in sin;
	memset (&sin, '\0', sizeof (sin));
	 
	/* 准备通信地址和端口号 */
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr (host); /* Server IP */
	sin.sin_port = htons (port); /* Server Port number */
	int icnn = connect (sock, (struct sockaddr *) &sin, sizeof (sin));
	if (icnn == -1)
	{
		printf ("can not connect to server,%s\n", strerror (errno));
		close(sock);
		return -202;
	}else{
		printf("[in cpp wrap -->] socket connect %d\n",sock);
	}
	/* 为ssl设置socket  */
	if (0 >= SSL_set_fd (ssl, sock))
	{
		printf ("Attach to Line fail!\n");
		close(sock);
		return -203;
	}

	retval = SSL_connect(ssl) ;
	printf("[in cpp wrap -->] SSL_connect retval = %d\n",retval);
	if (retval<= 0)
	{
		err = SSL_get_error(ssl, (int)retval);
		print_ssl_error(err,"Error SSL_connect");
		close(sock);
		return -204;
	}
	
	//连接成功，进入握手阶段
	
	
	long verify_flag = SSL_get_verify_result(ssl);
	//printf("SSL_get_verify_result %d\n",(int)verify_flag);
	if (verify_flag != X509_V_OK){
		printf("verify_flag != X509_V_O\n");
	} 	


	retval = SSL_do_handshake(ssl); //实测当有这一句时会发起握手

	printf("[in cpp wrap -->] do_handshake retval = %d\n",retval);
	if (retval <=0 )
	{
		err = SSL_get_error(ssl, (int)retval);
		print_ssl_error(err,(const char *)"SSL do handshake"); 
		close(sock);
		return -206;
	}

	return 0;
}

int TasslSockWrap::write(const char * buffer,const int len)
{
	printf("[in cpp wrap -->]:write len %d \n",len);
	int retval = 0;
	int err;
	retval = SSL_write(ssl, buffer, len);
	printf("[in cpp wrap -->]:after write  retval =  %d \n",retval);
	if(retval <=0)
	{
		err = SSL_get_error(ssl, (int)retval);
		print_ssl_error(err,(const char *)"when SSL_Write"); 
	}
	return retval;	
}

int TasslSockWrap::read(char *buffer, const int buffersize)
{
	//snprintf(buffer,buffersize,"[in cpp wrap -->] on read ,buffersize %d ",buffersize);
	int retval = 0;
	int err;
	retval = SSL_read(ssl,buffer,buffersize);
	printf("[in cpp wrap -->]:after read  retval =  %d \n",retval);
	if(retval <= 0){
		
		err = SSL_get_error(ssl, (int)retval);
		print_ssl_error(err,(const char *)"when SSL_read");
	}
	
	return retval;
}
