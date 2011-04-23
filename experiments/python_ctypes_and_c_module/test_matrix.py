import ctypes
import unittest

_rtgu = ctypes.CDLL("libRTGUcore.dylib")

mat44 = ctypes.c_float * 16
vec3  = ctypes.c_float * 3
_rtgu.rtgu_mat44_rotation_x.argtypes = [mat44, ctypes.c_float, ctypes.c_float]
_rtgu.rtgu_mat44_rotation_y.argtypes = [mat44, ctypes.c_float, ctypes.c_float]
_rtgu.rtgu_mat44_rotation_z.argtypes = [mat44, ctypes.c_float, ctypes.c_float]
_rtgu.rtgu_mul_mat44_vec3.argtypes = [mat44, vec3]

class Test(unittest.TestCase):
  
  def test_rotations(self):
    
    mat = mat44()
    X = vec3(1, 0, 0)
    Y = vec3(0, 1, 0)
    Z = vec3(0, 0, 1)
    
    # pi/2 rotation : rX * Y == Z
    
    vec = vec3(*Y)
    _rtgu.rtgu_mat44_rotation_x(mat, 0.0, 1.0)
    _rtgu.rtgu_mul_mat44_vec3(mat, vec)
    
    self.assert_( _rtgu.rtgu_eq_vec3(vec, Z) )

    # pi/2 rotation : rY * Z == X
    
    vec = vec3(*Z)
    _rtgu.rtgu_mat44_rotation_y(mat, 0.0, 1.0)
    _rtgu.rtgu_mul_mat44_vec3(mat, vec)
    
    self.assert_( _rtgu.rtgu_eq_vec3(vec, X) )

    # pi/2 rotation : rZ * X == Y
    
    vec = vec3(*X)
    _rtgu.rtgu_mat44_rotation_z(mat, 0.0, 1.0)
    _rtgu.rtgu_mul_mat44_vec3(mat, vec)
    
    self.assert_( _rtgu.rtgu_eq_vec3(vec, Y) )


if __name__ == "__main__":
  unittest.main()

