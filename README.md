# python_gmssl_wrap
为python语言包装国密的tls/ssl实现，包括cpp动态库，cython封装，测试代码等


目前进展：已经为python语言封装了TASSL库的几个关键接口，使python sdk可以通过国密tls连接节点，实现全流程国密化。

简单操作路径
1： 使用Tassl :https://github.com/jntass/TASSL

2： 针对tassl编写简单的c/cpp包装代码。

3： 用cython（http://docs.cython.org/ ）将c/cpp代码对接到python 


1) TASSL

如果不做代码和安装上的优化，则完全不用修改这个库本身，用就行了。
面向TASSL写了test_tassl.c的测试代码，可以全程跑通加载国密证书，和节点握手，进行通信的整个过程。

调试test_tassl.c的坑主要是BIO系列的api，尤其是BIO_do_handshake/BIO_write在当时环境不生效，导致莫名的握手失败。改用SSL_do_handshake/SSL_write则一次通过（注，TASSL封装了握手全过程，其实使用时无需操心）。
一个小坑：因为最终TASSL要作为动态库被python调，而它默认编译时不生成动态库，要执行 ./config 时增加参数 shared：
./config shared
make clean ；make ；make install



2) C/CPP封装代码

C代码见test/simplessl。

CPP代码见tassl_sock_wrap.cpp  test_tassl_sock.cpp

都很简单，最小功能集只用到SSL的初始化，加载证书，连接，握手，写/读等有限的接口。

当前版本保持最小化实现，pure c with class风格，尽量不用动态内存分配，new 内存buffer等，暂时没有必要。


后续增加更多选项接口，以及错误处理等。


3) 重点：cython

安装 pip install cython
相关的多语言包装器有很多个，包括SWIG,pybind11,Boost.Python等。选择cython全靠缘分，没有做深入比较，反正只是简单的胶水代码封装，都够用，没时间去纠结。

使用cython支持cpp->python只有简单的几步:

1：写一个后缀名为pxd的文件，映射到cpp 的class定义，基本上直接copy cpp的文件就差不多了。

2：写一个后缀名为pyx的文件，引用pxd里的定义，封装为python类，这里面可以使用一些类c/cpp的语法糖，相当于python语言的cpp超集。

3：写一个setup文件，编译它的时候要指定头文件和库文件目录。参见run_tassl.sh

参考：https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html cython和cpp整合专题。

但引用一些别人做好的案例模板，顺利的话，全套1小时以内就搞完了，高级语法再慢慢看。参考py_tassl_sock_wrap.pxd/pyx, setup_tassl_sock_wrap.py, run_tassl.sh等几个文件。很简单，再增加其他接口直接copy这几个文件或在里面加料即可。

编译时改的最多的bug居然是print/prinf，双引号单引号，有没有行末分号;等，混合编程最大的问题是语法混淆...


4) todolist:

和pythonsdk整合，去除多余打印，测试稳定性(尤其是长连接多线程等场景)，完善错误处理，增加日志，跨平台等...

