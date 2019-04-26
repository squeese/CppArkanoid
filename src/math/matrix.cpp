#include "matrix.h"

GLfloat& Mat4::operator [](unsigned int i) {
  return cell[i];
}

GLfloat Mat4::operator [](unsigned int i) const {
  return cell[i];
}

const Vec3 Mat4::operator *(const Vec3& o) const {
  GLfloat w = 1.0f / (cell[3] * o.x + cell[7] * o.y + cell[11] * o.z + cell[15]);
  return Vec3(
    (cell[0] * o.x + cell[4] * o.y + cell[8] * o.z  + cell[12]) * w,
    (cell[1] * o.x + cell[5] * o.y + cell[9] * o.z  + cell[13]) * w,
    (cell[2] * o.x + cell[6] * o.y + cell[10] * o.z + cell[14]) * w);
}

const Mat4 Mat4::operator *(const Mat4& o) const {
  Mat4 m;
  m[n11] = cell[n11] * o[n11] + cell[n12] * o[n21] + cell[n13] * o[n31] + cell[n14] * o[n41];
  m[n12] = cell[n11] * o[n12] + cell[n12] * o[n22] + cell[n13] * o[n32] + cell[n14] * o[n42];
  m[n13] = cell[n11] * o[n13] + cell[n12] * o[n23] + cell[n13] * o[n33] + cell[n14] * o[n43];
  m[n14] = cell[n11] * o[n14] + cell[n12] * o[n24] + cell[n13] * o[n34] + cell[n14] * o[n44];
  m[n21] = cell[n21] * o[n11] + cell[n22] * o[n21] + cell[n23] * o[n31] + cell[n24] * o[n41];
  m[n22] = cell[n21] * o[n12] + cell[n22] * o[n22] + cell[n23] * o[n32] + cell[n24] * o[n42];
  m[n23] = cell[n21] * o[n13] + cell[n22] * o[n23] + cell[n23] * o[n33] + cell[n24] * o[n43];
  m[n24] = cell[n21] * o[n14] + cell[n22] * o[n24] + cell[n23] * o[n34] + cell[n24] * o[n44];
  m[n31] = cell[n31] * o[n11] + cell[n32] * o[n21] + cell[n33] * o[n31] + cell[n34] * o[n41];
  m[n32] = cell[n31] * o[n12] + cell[n32] * o[n22] + cell[n33] * o[n32] + cell[n34] * o[n42];
  m[n33] = cell[n31] * o[n13] + cell[n32] * o[n23] + cell[n33] * o[n33] + cell[n34] * o[n43];
  m[n34] = cell[n31] * o[n14] + cell[n32] * o[n24] + cell[n33] * o[n34] + cell[n34] * o[n44];
  m[n41] = cell[n41] * o[n11] + cell[n42] * o[n21] + cell[n43] * o[n31] + cell[n44] * o[n41];
  m[n42] = cell[n41] * o[n12] + cell[n42] * o[n22] + cell[n43] * o[n32] + cell[n44] * o[n42];
  m[n43] = cell[n41] * o[n13] + cell[n42] * o[n23] + cell[n43] * o[n33] + cell[n44] * o[n43];
  m[n44] = cell[n41] * o[n14] + cell[n42] * o[n24] + cell[n43] * o[n34] + cell[n44] * o[n44];
  return m;
}

const Mat4 Mat4::Translation(const Vec3& translation) {
  Mat4 m;
  m[n14] = translation.x;
  m[n24] = translation.y;
  m[n34] = translation.z;
  return m;
}

void Mat4::Translate(GLfloat x, GLfloat y, GLfloat z) {
  cell[n14] += x;
  cell[n24] += y;
  cell[n34] += z;
}

const Mat4 Mat4::Translation(GLfloat x, GLfloat y, GLfloat z) {
  Mat4 m;
  m[n14] = x;
  m[n24] = y;
  m[n34] = z;
  return m;
}

const Mat4 Mat4::EulerRotationZXY(const Vec3& euler) {
  return Mat4::EulerRotationZXY(euler.x, euler.y, euler.z);
}

const Mat4 Mat4::EulerRotationZXY(GLfloat x, GLfloat y, GLfloat z) {
  GLfloat a = cos(x);
  GLfloat b = sin(x);
  GLfloat c = cos(y);
  GLfloat d = sin(y);
  GLfloat e = cos(z);
  GLfloat f = sin(z);
  GLfloat ce = c * e;
  GLfloat cf = c * f;
  GLfloat de = d * e;
  GLfloat df = d * f;
  Mat4 m;
  m[n11] = ce - df * b;
  m[n12] = -a * f;
  m[n13] = de + cf * b;
  m[n21] = cf + de * b;
  m[n22] = a * e;
  m[n23] = df - ce * b;
  m[n31] = -a * d;
  m[n32] = b;
  m[n33] = a * c;
  return m;
}

const Mat4 Mat4::Scale(const Vec3& scalars) {
  Mat4 m;
  m[n11] = scalars.x;
  m[n22] = scalars.y;
  m[n33] = scalars.z;
  return m;
}

const Mat4 Mat4::Scale(GLfloat x, GLfloat y, GLfloat z) {
  Mat4 m;
  m[n11] = x;
  m[n22] = y;
  m[n33] = z;
  return m;
}

const Mat4 Mat4::Identity() {
  Mat4 m;
  return m;
}

const Mat4 Mat4::Transform(const Vec3& position, const Vec3& rotation, const Vec3& scale) {
  Mat4 mTranslation = Mat4::Translation(position);
  Mat4 mRotation = Mat4::EulerRotationZXY(rotation);
  Mat4 mScale = Mat4::Scale(scale);
  return mScale * mRotation * mTranslation;
}

const Mat4 Mat4::Perspective(GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far) {
  Mat4 m;
  GLfloat C = 1.0f / tan(fov * 0.5f);
  GLfloat R = 1.0f / (near - far);
  m[n11] = C/ratio;
  m[n22] = C;
  m[n33] = (far + near) * R;
  m[n34] = 2.0f * near * far * R;
  m[n43] = -1.0f;
  m[n44] = 0.0f;
  return m;
}

const Mat4 Mat4::LookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
  Vec3 F = (target - eye).Normalized();
  Vec3 R = Vec3::Cross(up, F).Normalized();
  Vec3 U = Vec3::Cross(F, R);
  Mat4 m;
  m[n11] = R.x;  m[n12] = U.x;  m[n13] = F.x;  m[n14] = eye.x;
  m[n21] = R.y;  m[n22] = U.y;  m[n23] = F.y;  m[n24] = eye.y;
  m[n31] = R.z;  m[n32] = U.z;  m[n33] = F.z;  m[n34] = eye.z;
  return m;
}
