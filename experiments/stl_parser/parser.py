import ctypes
import os
import struct
import time

class vec3(ctypes.Structure):
  _fields_ = [
    ("x", ctypes.c_float),
    ("y", ctypes.c_float),
    ("z", ctypes.c_float),
  ]

class Triangle(ctypes.LittleEndianStructure):
  _pack_ = 1
  _fields_ = [
    ("normal", vec3)
  , ("v", vec3 * 3)
  , ("attribByteCount", ctypes.c_uint16)
  ]

###

def unpack(CType, buffer):
  sizeof = ctypes.sizeof(CType)
  result = CType()
  ctypes.memmove( ctypes.addressof(result), buffer, sizeof )
  return result

def unpackN(output, buffer):
  sizeof = ctypes.sizeof(output)
  ctypes.memmove( ctypes.addressof(output), buffer, sizeof )

###
###

t0 = time.time()

verts = set()

with file("m02parties.stl", "r") as f:
  
  header = f.read(80)
  numTri = struct.unpack( "<I", f.read(4) )[0]
  sizeofTri = ctypes.sizeof(Triangle)
  #numTri = min(numTri, 65536*8)
  print numTri, "triangles"
  
  BATCH_TRI_SIZE = 8192
  
  remaining = numTri
  while remaining > 0:
    batch = min(remaining, BATCH_TRI_SIZE)

    triangles = (Triangle * batch)()
    numBytes = ctypes.sizeof(triangles)

    unpackN( triangles, f.read(numBytes) )
    remaining -= batch
    
    for tri in triangles:
      for v in tri.v:
        hv = (v.x, v.y, v.z)
        verts.add(hv)

##

t1 = time.time()

print len(verts), "vertices", "/", numTri*3, "=", (100 * len(verts) / (numTri*3)), "%"
print "done.", t1-t0, "sec"
