#ifndef RTGU_MATRIX_H
#define RTGU_MATRIX_H

typedef float rtgu_mat44[16];

void rtgu_mat44_identity(rtgu_mat44 m);

void rtgu_mat44_rotation_x(rtgu_mat44 m, float cosx, float sinx);
void rtgu_mat44_rotation_y(rtgu_mat44 m, float cosx, float sinx);
void rtgu_mat44_rotation_z(rtgu_mat44 m, float cosx, float sinx);

void rtgu_mat44_scale(rtgu_mat44 m, float x, float y, float z);

void rtgu_mat44_translation(rtgu_mat44 m, float x, float y, float z);

void rtgu_mul_mat44_vec3(rtgu_mat44 m, float vec3[3]);

//

int rtgu_eq_vec3(float a[3], float b[3]);

#endif

