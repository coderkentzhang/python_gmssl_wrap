# cython: language_level=3
# Declare the class with cdef
cdef extern from "tassl_sock_wrap.h" namespace "fisco_tassl_sock_wrap":
	cdef cppclass TasslSockWrap:
		TasslSockWrap() except +
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