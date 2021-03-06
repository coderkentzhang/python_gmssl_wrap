
from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "py_tassl_sock_wrap",
    ["py_tassl_sock_wrap.pyx"], 
    libraries=["tassl_sock_wrap","ssl","crypto"] 
)

setup(
    ext_modules=cythonize([extension])
)