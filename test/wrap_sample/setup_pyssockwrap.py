from distutils.core import setup, Extension
from Cython.Build import cythonize

extension = Extension(
    "py_ssock_wrap",
    ["py_ssock_wrap.pyx"], 
    libraries=["ssock_wrap"] 
)

setup(
    ext_modules=cythonize([extension])
)