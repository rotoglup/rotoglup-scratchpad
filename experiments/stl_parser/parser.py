import ctypes
import os
import struct

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

###
###

verts = set()

with file("m02parties.stl", "r") as f:
  
  header = f.read(80)
  numTri = struct.unpack( "<I", f.read(4) )[0]
  sizeofTri = ctypes.sizeof(Triangle)
  #numTri = min(numTri, 65536*16)
  print numTri, "triangles"
  for i in xrange(numTri):
    tri = unpack(Triangle, f.read(sizeofTri))
    for v in tri.v:
      hv = (v.x, v.y, v.z)
      verts.add(hv)

###

print "done", len(verts), "vertices", "/", numTri*3, "=", (100 * len(verts) / (numTri*3)), "%"
