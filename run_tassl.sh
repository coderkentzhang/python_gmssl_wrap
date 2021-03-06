make
#rm py_tassl_sock_wrap.cpython-37m-x86_64-linux-gnu.so -f
echo ""
echo "build python Extension"
CFLAGS="-I`pwd` -I../TASSL/include/openssl" LDFLAGS="-L`pwd` -L../TASSL"  python setup_tassl_sock_wrap.py build_ext --inplace

echo "start test -->"
echo ""
python test_tassl_sock_wrap.py	
echo ""
echo "<--test done"