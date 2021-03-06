LINUX_CFLAGS=-Wall -pedantic -g -O2
LINUX_LIBS=

all: lib test 

test: wrap_test 
lib: libssock_wrap.so

#common flags
CC=gcc
SYSCFLAGS=
SYSLIBS=
LDFLAGS=$(SYSLIBS)
INCLUDES=
CFLAGS=$(SYSCFLAGS) $(INCLUDES)
LINUX_CFLAGS=-Wall -pedantic -g -O2




TEST_SOURCES=wrap_test.cpp
TEST_OBJS=wrap_test.o
TEST_CFLAGS=$(CFLAGS)
TEST_LDFLAGS=-lstdc++ -L./ -lssock_wrap 
wrap_test: $(TEST_SOURCES) libssock_wrap.so
	@echo "-->Build test"
	$(CC) $(TEST_CFLAGS) $(LINUX_CFLAGS) -c wrap_test.cpp -o wrap_test.o
	$(CC) $(TEST_CFLAGS) $(TEST_LDFLAGS) $(TEST_OBJS) -o wrap_test



LIB_SOURCES=ssock_wrap.cpp 
LIB_OBJS=ssock_wrap.o
LIB_CFLAGS=$(CFLAGS) 
LIB_LDFLAGS=-pthread -shared  
libssock_wrap.so: $(LIB_SOURCES) 
	@echo "-->Build so lib"
	$(CC) $(LIB_CFLAGS) $(LINUX_CFLAGS) -fPIC -c ssock_wrap.cpp
	$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS) $(LIB_OBJS) -o libssock_wrap.so
	
	

clean:
	@rm -f wrap_test
	@rm -f libssock_wrap.so
	@rm -f ssock_wrap.o
	@rm -f *.o
	@rm -f *.so