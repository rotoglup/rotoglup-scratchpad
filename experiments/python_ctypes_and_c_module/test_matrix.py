import ctypes
import unittest

_rtgu = ctypes.CDLL("libRTGUcore.dylib")

mat44 = ctypes.c_float * 16
vec3  = ctypes.c_float * 3
_rtgu.rtgu_mat44_rotationX.argtypes = [mat44, ctypes.c_float, ctypes.c_float]
_rtgu.rtgu_mul_mat44_vec3.argtypes = [mat44, vec3]

mat = mat44()
X = vec3(1, 0, 0)
Y = vec3(0, 1, 0)
Z = vec3(0, 0, 1)
vec = vec3(*Y)

_rtgu.rtgu_mat44_identity(mat)
_rtgu.rtgu_mat44_rotationX(mat, 0.0, 1.0) # pi/2 rotation
_rtgu.rtgu_mul_mat44_vec3(mat, vec)
print [ f for f in mat ]
print [ f for f in vec ]

class Test(unittest.TestCase):
  
  def test_rotations(self):
    
    mat = mat44()
    X = vec3(1, 0, 0)
    Y = vec3(0, 1, 0)
    Z = vec3(0, 0, 1)
    vec = vec3(*Y)
    
    _rtgu.rtgu_mat44_rotationX(mat, 0.0, 1.0) # pi/2 rotation
    _rtgu.rtgu_mul_mat44_vec3(mat, vec)
    
    self.assert_( _rtgu.rtgu_eq_vec3(vec, Z) )


if __name__ == "__main__":
  unittest.main()

