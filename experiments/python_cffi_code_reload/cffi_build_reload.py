import gc
import os
_THIS_DIR = os.path.dirname( os.path.abspath(__file__) )

import cffi

_API_H = os.path.join( _THIS_DIR, "api.h" )
_DLL_NAME = "api"
_DLL_FILE = "%s.dll" % _DLL_NAME

#=============================================================================
# Build
#

ffi = cffi.FFI()
ffi.set_source("my_plugin", open("api.c").read(), extra_compile_args=['/MT'])
ffi.compile(target="%s.*" % _DLL_NAME, verbose=True)

#=============================================================================
# Load
#

ffi = cffi.FFI()

api_path = _API_H
lib_path = _DLL_FILE
with open(api_path) as f:
  ffi.cdef(f.read())

mylib = ffi.dlopen(lib_path)

assert 12 == mylib.return_some_value()

#=============================================================================
# Unload
#

del mylib
del ffi
gc.collect()

#=============================================================================
# Rebuild
#

ffi = cffi.FFI()
ffi.set_source("my_plugin", open("api_v2.c").read(), extra_compile_args=['/MT'])
ffi.compile(target="%s.*" % _DLL_NAME, verbose=True)

#=============================================================================
# Reload
#

ffi = cffi.FFI()

api_path = _API_H
lib_path = _DLL_FILE
with open(api_path) as f:
  ffi.cdef(f.read())

mylib = ffi.dlopen(lib_path)

assert 13 == mylib.return_some_value()
