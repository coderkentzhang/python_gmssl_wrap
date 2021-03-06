#ifndef PY_SSOCK_WRAP
#define PY_SSOCK_WRAP

namespace py_ssock_wrap{
	
	class SSockWrap{
		
	public :
		SSockWrap();
		~SSockWrap();
		int init();
		int shutdown();
		int close();
		int write(const char * buffer,const int len);
		int read(char *buffer,const int buffersize);
			
	};
	
	
}

#endif