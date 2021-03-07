# python_gmssl_wrap
为python语言包装国密的tls/ssl实现，包括cpp动态库，cython封装，测试代码等


目前进展：已经为python语言封装了TASSL库的几个关键接口，使python sdk可以通过国密tls连接节点，实现全流程国密化。

简单操作路径
1： 使用Tassl :https://github.com/jntass/TASSL

2： 针对tassl编写简单的c/cpp包装代码。

3： 用cython（http://docs.cython.org/ ）将c/cpp代码对接到python 


1) TASSL

如果不做代码和安装上的优化，则完全不用修改这个库本身，用就行了。
面向TASSL写了test_tassl.c的测试代码(test/simplessl)，可以全程跑通加载国密证书，和节点握手，进行通信的整个过程。

调试test_tassl.c的坑主要是BIO系列的api，尤其是BIO_do_handshake/BIO_write在当时环境不生效，导致莫名的握手失败。改用SSL_do_handshake/SSL_write则一次通过（注，TASSL封装了握手全过程，其实使用时无需操心）。
因为最终TASSL要作为动态库被python调，而它默认编译时不生成动态库，要执行 ./config 时增加参数 shared：
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

1：写一个后缀名为pxd的文件，映射到cpp 的class定义，基本上直接copy cpp的头文件就差不多了。

2：写一个后缀名为pyx的文件，引用pxd里的定义，封装为python类，这里面可以使用一些类c/cpp的语法糖，相当于python语言的cpp超集。

3：写一个setup_xxx.py文件，运行它生成扩展库，命令行要指定头文件和库文件目录。

最后，运行Python测试代码时可能要指定LD_LIBRARY_PATH....环境变量以定位各种so库，取决于环境。


参考：https://cython.readthedocs.io/en/latest/src/userguide/wrapping_CPlusPlus.html cython和cpp整合专题。

引用一些做好的案例模板，顺利的话，全套1小时以内就搞完了，参考py_tassl_sock_wrap.pxd/pyx, setup_tassl_sock_wrap.py, run_tassl.sh等几个文件。很简单，再增加其他接口直接copy这几个文件或在里面加料即可。

参考py_tassl_sock_wrap.pxd/pyx, setup_tassl_sock_wrap.py, run_tassl.sh等几个文件。很简单，再增加其他接口直接copy这几个文件或在里面加料即可。

高级语法再慢慢看。python和c相关的一些细节方法：

https://docs.python.org/3.7/extending/index.html  可切换到中文
https://docs.python.org/zh-cn/3.7/extending/index.html
bytes转换之类的参见:https://docs.python.org/3.7/c-api/bytes.html?highlight=pybytes_fromstringandsize

理论上可以使用cython实现python直接调用tassl库的c语言接口（大部分在ssl.h），无需再写中间的cpp（实际上是把c代码用cython语法写到pyx文件里），考虑到python和c/cpp有各种语义差异（结构体，函数指针，模板等），这种直接调的写法不如直接写一个直观的cpp 类先屏蔽一遍数据类型和其他细节，收敛到有限接口，更加愉快，只是多个so文件而已。

4) todolist:

和python sdk整合，去除多余打印，测试稳定性(尤其是长连接多线程等场景)，完善错误处理，增加日志，跨平台等...

