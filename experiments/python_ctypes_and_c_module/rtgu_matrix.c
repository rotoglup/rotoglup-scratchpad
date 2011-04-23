#include "rtgu_matrix.h"

#define _M(row, col) (m)[(col)+((row)*4)]

void rtgu_mat44_identity(rtgu_mat44 m)
{
  _M(0,0) = 1.0f; _M(0,1) = 0.0f; _M(0,2) = 0.0f; _M(0,3) = 0.0f;
  _M(1,0) = 0.0f; _M(1,1) = 1.0f; _M(1,2) = 0.0f; _M(1,3) = 0.0f;
  _M(2,0) = 0.0f; _M(2,1) = 0.0f; _M(2,2) = 1.0f; _M(2,3) = 0.0f;
  _M(3,0) = 0.0f; _M(3,1) = 0.0f; _M(3,2) = 0.0f; _M(3,3) = 1.0f;
}

void rtgu_mat44_rotation_x(rtgu_mat44 m, float cosx, float sinx)
{
  _M(0,0) = 1.0f; _M(0,1) = 0.0f; _M(0,2) = 0.0f; _M(0,3) = 0.0f;
  _M(1,0) = 0.0f; _M(1,1) = cosx; _M(1,2) =-sinx; _M(1,3) = 0.0f;
  _M(2,0) = 0.0f; _M(2,1) = sinx; _M(2,2) = cosx; _M(2,3) = 0.0f;
  _M(3,0) = 0.0f; _M(3,1) = 0.0f; _M(3,2) = 0.0f; _M(3,3) = 1.0f;
}

void rtgu_mat44_rotation_y(rtgu_mat44 m, float cosx, float sinx)
{
  _M(0,0) = cosx; _M(0,1) = 0.0f; _M(0,2) = sinx; _M(0,3) = 0.0f;
  _M(1,0) = 0.0f; _M(1,1) = 1.0f; _M(1,2) = 0.0f; _M(1,3) = 0.0f;
  _M(2,0) =-sinx; _M(2,1) = 0.0f; _M(2,2) = cosx; _M(2,3) = 0.0f;
  _M(3,0) = 0.0f; _M(3,1) = 0.0f; _M(3,2) = 0.0f; _M(3,3) = 1.0f;
}

void rtgu_mat44_rotation_z(rtgu_mat44 m, float cosx, float sinx)
{
  _M(0,0) = cosx; _M(0,1) =-sinx; _M(0,2) = 0.0f; _M(0,3) = 0.0f;
  _M(1,0) = sinx; _M(1,1) = cosx; _M(1,2) = 0.0f; _M(1,3) = 0.0f;
  _M(2,0) = 0.0f; _M(2,1) = 0.0f; _M(2,2) = 1.0f; _M(2,3) = 0.0f;
  _M(3,0) = 0.0f; _M(3,1) = 0.0f; _M(3,2) = 0.0f; _M(3,3) = 1.0f;
}

void rtgu_mat44_scale(rtgu_mat44 m, float x, float y, float z)
{
  _M(0,0) =    x; _M(0,1) = 0.0f; _M(0,2) = 0.0f; _M(0,3) = 0.0f;
  _M(1,0) = 0.0f; _M(1,1) =    y; _M(1,2) = 0.0f; _M(1,3) = 0.0f;
  _M(2,0) = 0.0f; _M(2,1) = 0.0f; _M(2,2) =    z; _M(2,3) = 0.0f;
  _M(3,0) = 0.0f; _M(3,1) = 0.0f; _M(3,2) = 0.0f; _M(3,3) = 1.0f;
}

void rtgu_mat44_translation(rtgu_mat44 m, float x, float y, float z)
{
  _M(0,0) = 1.0f; _M(0,1) = 0.0f; _M(0,2) = 0.0f; _M(0,3) = x;
  _M(1,0) = 0.0f; _M(1,1) = 1.0f; _M(1,2) = 0.0f; _M(1,3) = y;
  _M(2,0) = 0.0f; _M(2,1) = 0.0f; _M(2,2) = 1.0f; _M(2,3) = z;
  _M(3,0) = 0.0f; _M(3,1) = 0.0f; _M(3,2) = 0.0f; _M(3,3) = 1.0f;
}

void rtgu_mul_mat44_vec3(rtgu_mat44 m, float vec3[3])
{
  float r[3];
  r[0] = _M(0,0) * vec3[0] + _M(0,1) * vec3[1] + _M(0,2) * vec3[2];
  r[1] = _M(1,0) * vec3[0] + _M(1,1) * vec3[1] + _M(1,2) * vec3[2];
  r[2] = _M(2,0) * vec3[0] + _M(2,1) * vec3[1] + _M(2,2) * vec3[2];
  vec3[0] = r[0];
  vec3[1] = r[1];
  vec3[2] = r[2];
}

int rtgu_eq_vec3(float a[3], float b[3])
{
  return (
       a[0] == b[0]
    && a[1] == b[1]
    && a[2] == b[2]
  );
}

