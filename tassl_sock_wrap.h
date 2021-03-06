#ifndef TASSL_SOCK_WRAP
#define TASSL_SOCK_WRAP
#include <stdlib.h>
#include <openssl/ssl.h>


namespace fisco_tassl_sock_wrap{
	
	class TasslSockWrap{
		char host[256];
		int port;
		char ca_crt_file[256];
		char sign_crt_file[256];
		char sign_key_file[256];
		char en_crt_file[256];
		char en_key_file[256];
		SSL *ssl ;
		SSL_CTX *ctx ;
		int  load_ca_files();
	public :
		TasslSockWrap();
		~TasslSockWrap(); 
		int init();
		void set_host_port(const char *host_, int port_);

		void set_ca_files(const char *ca_crt_file_,
						const char * sign_crt_file_,
						const char * sign_key_file_,
						const char * en_crt_file_,
						const char * en_key_file_
							);

		int try_connect();
		int finish();
		
		
		int write(const char * buffer,const int len);
		int read(char *buffer,const int buffersize);
	};
	
	
}

#endif