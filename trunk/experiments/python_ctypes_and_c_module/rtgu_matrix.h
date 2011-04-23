#ifndef RTGU_MATRIX_H
#define RTGU_MATRIX_H

typedef float rtgu_mat44[16];

void rtgu_mat44_identity(rtgu_mat44 m);

void rtgu_mat44_rotationX(rtgu_mat44 m, float cosx, float sinx);

void rtgu_mul_mat44_vec3(rtgu_mat44 m, float vec3[3]);

//

int rtgu_eq_vec3(float a[3], float b[3]);

#endif

