# cython: language_level=3
# distutils: language = c++


from cpython.bytes cimport PyBytes_FromStringAndSize
from py_tassl_sock_wrap cimport TasslSockWrap



cdef class PyTasslSockWrap:
	cdef TasslSockWrap ssock  # Hold a C++ instance which we're wrapping

	def __cinit__(self):
		self.ssock =  TasslSockWrap()

		
	def __dealloc__(self):
		pass
		
	def init(self):
		self.ssock.init()
		
	def finish(self):
		self.ssock.finish()
		  
		
	def set_host_port(self,host_, port_):
		self.ssock.set_host_port(host_.encode("UTF-8"),port_)

	def set_ca_files(self,
					ca_crt_file_,
					sign_crt_file_,
					sign_key_file_,
					en_crt_file_,
					en_key_file_
						):
		self.ssock.set_ca_files(
					ca_crt_file_.encode("UTF-8"),
					sign_crt_file_.encode("UTF-8"),
					sign_key_file_.encode("UTF-8"),
					en_crt_file_.encode("UTF-8"),
					en_key_file_.encode("UTF-8")
					)	
		
	def try_connect(self,):
		self.ssock.try_connect()

	def write(self,buffer: bytes,bufferlen=None):
		print("{ in pyx -->} ssock write :",buffer)
		if bufferlen is  None:
			bufferlen = len(buffer)
		r = self.ssock.write(buffer,bufferlen)
		return r;

	MAX_RECV_BUFF = 10240
	def read(self,buffersize=MAX_RECV_BUFF) -> bytes:
		print("{ in pyx -->} ssock read :",buffersize)
		cdef char buffer[10240];
		r = self.ssock.read(buffer,buffersize)
		print("{ in pyx -->} after read ,len ", r )
		bytebuffer = PyBytes_FromStringAndSize(buffer,r)
		print(bytebuffer)
		return bytebuffer 
		