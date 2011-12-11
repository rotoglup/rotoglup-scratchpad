# -*- coding: windows-1252 -*-

import ctypes

##
##
##

class HEADER(ctypes.Structure):
  _fields_ = [
    ("num_vtx", ctypes.c_uint32),
    ("num_tri", ctypes.c_uint32),
    ("aabb_min", ctypes.c_float * 3),
    ("aabb_max", ctypes.c_float * 3),
    ("vtx_data_len", ctypes.c_uint32),
    ("idx_data_len", ctypes.c_uint32),
    ("gl_index_type", ctypes.c_uint32),
    ("gl_num_attribs", ctypes.c_uint32),
  ]

class ATTRIB_DESC(ctypes.Structure):
  _fields_ = [
    ("id", ctypes.c_uint32),
    ("size", ctypes.c_uint32),
    ("type", ctypes.c_uint32),
    ("normalized", ctypes.c_uint32),
    ("stride", ctypes.c_uint32),
    ("offset", ctypes.c_uint32),
  ]

##
##
##

def consumeBytes(count, binaryStream):
  
  result = binaryStream[:count]
  return result, binaryStream[count:]


def unpackConsume(Type, binaryStream):
  
  sizeof = ctypes.sizeof(Type)
  if len(binaryStream) < sizeof:
    raise RuntimeError("Invalid binary content")
  
  data, binaryStream = consumeBytes(sizeof, binaryStream)
  
  result = Type()
  ctypes.memmove( ctypes.addressof(result), data, sizeof )

  return result, binaryStream

##
##
##

def readMesh( filename ):

  import py_mgOpenGL as GL
  import gl_shader

  with open( filename, "rb" ) as input:
    binary = input.read()

  # Parse header
  #
  in_header, binary = unpackConsume(HEADER, binary)
  
  mesh = Mesh() 
  mesh.numVertices = in_header.num_vtx
  mesh.numIndices = in_header.num_tri * 3
  mesh.indexGLtype = in_header.gl_index_type
  mesh.glPrimType = GL.GL_TRIANGLES
  
  mesh.aabb = ( tuple(in_header.aabb_min[:]), tuple(in_header.aabb_max[:]) )
  
  # parse attributes descriptions
  
  attrib_desc_t = (ATTRIB_DESC * in_header.gl_num_attribs)
  in_attrib_desc, binary = unpackConsume(attrib_desc_t, binary)
  
  mesh.attribs = []
  for value in in_attrib_desc:
    as_tuple = (value.id, value.size, value.type, value.normalized, value.stride, value.offset)
    mesh.attribs.append( as_tuple )

  # convert buffers to ctypes

  vertexData = binary[:in_header.vtx_data_len]
  binary = binary[in_header.vtx_data_len:]

  indexData = binary[:in_header.idx_data_len]
  binary = binary[in_header.idx_data_len:]
  
  assert( 0 == len(binary) )

  #
    
  #
  
  mesh.arrayBuffer = gl_shader.GLBuffer( GL.glGenBuffer() )
  mesh.elementArrayBuffer = gl_shader.GLBuffer( GL.glGenBuffer() )

  return (mesh, vertexData, indexData)


class Mesh:
  
  def __init__(self):
    
    self.arrayBuffer = None
    self.elementArrayBuffer = None
    self.numVertices = 0
    self.numIndices = 0
    self.indexGLtype = 0
    self.glPrimType = 0
    self.attribs = []
    self.aabb = None
    
  def dispose(self):
    
    if self.arrayBuffer is not None:
      self.arrayBuffer.dispose()
    
    if self.elementArrayBuffer is not None:
      self.elementArrayBuffer.dispose()


if __name__ == "__main__":

  import json
  import sys

  filename = sys.argv[1]
    
  with open( filename, "rb" ) as input:
    binary = input.read()

  # Parse header & attrib description
  #
  header, binary = unpackConsume(HEADER, binary)
  
  attrib_desc_t = (ATTRIB_DESC * header.gl_num_attribs)
  attrib_desc, binary = unpackConsume(attrib_desc_t, binary)

  vertexData = binary[:header.vtx_data_len]
  binary = binary[header.vtx_data_len:]

  indexData = binary[:header.idx_data_len]
  binary = binary[header.idx_data_len:]
  
  print [ ord(x) for x in indexData[:8] ]
  
  assert( 0 == len(binary) )

  #

  class CTypesEncoder(json.JSONEncoder):
    
    def default(self, obj):
      
      if isinstance(obj, ctypes.Structure):

        jsonDict = {}
        for name in obj.__dict__:
          value = getattr(obj, name)
          jsonDict[name] = value

        for field in obj._fields_:
          name, type = field
          value = getattr(obj, name)
          jsonDict[name] = value

        return jsonDict

      elif isinstance(obj, ctypes.Array):
        
        return [ item for item in obj ]
      
      print obj.__class__
      return json.JSONEncoder.default(self, obj)


  header.data_offset = ctypes.sizeof(HEADER) + ctypes.sizeof(attrib_desc_t)
  print CTypesEncoder().encode( [ header, attrib_desc ] )
  print ""
