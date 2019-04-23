#include "shapes.h"
#include <cmath>

Shape::~Shape() {
  delete vertices;
  delete normals;
  delete triangles;
  delete uvs;
}

void ShapeBuilder::SetVertex(GLfloat x, GLfloat y, GLfloat z) {
  shape->vertices[vertexIndex].x = x;
  shape->vertices[vertexIndex].y = y;
  shape->vertices[vertexIndex].z = z;
  vertexIndex++;
}

template<typename T>
void ShapeBuilder::SetVertex(GLfloat x, GLfloat y, GLfloat z, T index) {
  shape->vertices[index] = Vec3 { x, y, z };
}

template<typename T, typename... Args>
void ShapeBuilder::SetVertex(GLfloat x, GLfloat y, GLfloat z, T index, Args... args) {
  shape->vertices[index] = Vec3 { x, y, z };
  SetVertex(x, y, z, args...);
}

void ShapeBuilder::SetTriangle(GLushort a, GLushort b, GLushort c) {
  shape->triangles[triangleIndex].a = a;
  shape->triangles[triangleIndex].b = b;
  shape->triangles[triangleIndex].c = c;
  triangleIndex++;
}

void ShapeBuilder::SetUv(GLfloat x, GLfloat y) {
  shape->uvs[uvIndex].x = x;
  shape->uvs[uvIndex].y = y;
  uvIndex++;
}

template<typename T>
void ShapeBuilder::SetUv(GLfloat x, GLfloat y, T index) {
  shape->uvs[index] = Vec2 { x, y };
}

template<typename T, typename... Args>
void ShapeBuilder::SetUv(GLfloat x, GLfloat y, T index, Args... args) {
  shape->uvs[index] = Vec2 { x, y };
  SetUv(x, y, args...);
}

template<typename T>
void ShapeBuilder::SetNormal(GLfloat x, GLfloat y, GLfloat z, T index) {
  shape->normals[index] = Vec3 { x, y, z };
}

template<typename T, typename... Args>
void ShapeBuilder::SetNormal(GLfloat x, GLfloat y, GLfloat z, T index, Args... args) {
  shape->normals[index] = Vec3 { x, y, z };
  SetNormal(x, y, z, args...);
}

void ShapeBuilder::SetNormal(unsigned int a, unsigned int b) {
  const Vec3* vo = &shape->vertices[normalIndex];
  const Vec3 va = shape->vertices[a] - *vo;
  const Vec3 vb = shape->vertices[b] - *vo;
  shape->normals[normalIndex] = Vec3::Cross(va, vb).Normalized();
  normalIndex++;
}

void ShapeBuilder::SetNormal(unsigned int a, unsigned int b, unsigned c) {
  const Vec3* vo = &shape->vertices[normalIndex];
  const Vec3 va = shape->vertices[a] - *vo;
  const Vec3 vb = shape->vertices[b] - *vo;
  const Vec3 vc = shape->vertices[c] - *vo;
  const Vec3 vab = Vec3::Cross(va, vb);
  const Vec3 vbc = Vec3::Cross(vb, vc);
  const Vec3 vca = Vec3::Cross(vc, va);
  shape->normals[normalIndex] = (vab + vbc + vca).Normalized();
  normalIndex++;
}

std::shared_ptr<Shape> ShapeBuilder::BoxRound(float width, float height, float depth) {
  ShapeBuilder builder(8, 12);
  float x = abs(width / 2.0f);
  float y = abs(height / 2.0f);
  float z = abs(depth / 2.0f);
  builder.SetVertex(-x, +y, +z); // Front-Top-Left         0---------1
  builder.SetVertex(+x, +y, +z); // Front-Top-Right       /|        /|
  builder.SetVertex(-x, -y, +z); // Front-Bot-Left       4 | - - - 5 |
  builder.SetVertex(+x, -y, +z); // Front-Bot-Right      ; |       : |
  builder.SetVertex(-x, +y, -z); // Back-Top-Left        ; 2---------3
  builder.SetVertex(+x, +y, -z); // Back-Top-Right       ;/        :/
  builder.SetVertex(-x, -y, -z); // Back-Bot-Left        6 - - - - 7
  builder.SetVertex(+x, -y, -z); // Back-Bot-Right
  builder.SetNormal(1, 4, 2);
  builder.SetNormal(0, 3, 5);
  builder.SetNormal(0, 6, 3);
  builder.SetNormal(1, 2, 7);
  builder.SetNormal(0, 5, 6);
  builder.SetNormal(1, 7, 4);
  builder.SetNormal(2, 4, 7);
  builder.SetNormal(3, 6, 5);
  builder.SetUv(0.0f, 0.0f);
  builder.SetUv(1.0f, 0.0f);
  builder.SetUv(0.0f, 1.0f);
  builder.SetUv(1.0f, 1.0f);
  builder.SetUv(1.0f, 0.0f);
  builder.SetUv(0.0f, 0.0f);
  builder.SetUv(1.0f, 1.0f);
  builder.SetUv(0.0f, 1.0f);
  builder.SetTriangle(0, 2, 3);
  builder.SetTriangle(0, 3, 1);
  builder.SetTriangle(0, 4, 6);
  builder.SetTriangle(0, 6, 2);
  builder.SetTriangle(0, 1, 5);
  builder.SetTriangle(0, 5, 4);
  builder.SetTriangle(7, 3, 2);
  builder.SetTriangle(7, 2, 6);
  builder.SetTriangle(7, 6, 4);
  builder.SetTriangle(7, 4, 5);
  builder.SetTriangle(7, 5, 1);
  builder.SetTriangle(7, 1, 3);
  return std::shared_ptr<Shape>(builder.shape);
}

std::shared_ptr<Shape> ShapeBuilder::BoxFlat(float width, float height, float depth) {
  ShapeBuilder builder(24, 12);
  float x = abs(width / 2.0f);
  float y = abs(height / 2.0f);
  float z = abs(depth / 2.0f);
  builder.SetVertex(-x, +y, +z, 0,  1,  2);
  builder.SetVertex(+x, +y, +z, 3,  4,  5);
  builder.SetVertex(-x, -y, +z, 6,  7,  8);
  builder.SetVertex(+x, -y, +z, 9,  10, 11);
  builder.SetVertex(-x, +y, -z, 12, 13, 14);
  builder.SetVertex(+x, +y, -z, 15, 16, 17);
  builder.SetVertex(-x, -y, -z, 18, 19, 20);
  builder.SetVertex(+x, -y, -z, 21, 22, 23);
  //               12----15
  //               |      |
  //               |      |
  //               2------4
  //      13-----1 0------3 5-----16 17----14
  //      |      | |      | |      | |      |
  //      |      | |      | |      | |      |
  //      18-----7 6------9 10----22 23----20
  //               8-----11
  //               |      |
  //               |      |
  //               19----21
  builder.SetNormal(0.0f, 0.0f, 1.0f, 0, 3, 6, 9);
  builder.SetNormal(0.0f, 1.0f, 0.0f, 2, 4, 12, 15);
  builder.SetNormal(1.0f, 0.0f, 0.0f, 5, 16, 10, 22);
  builder.SetNormal(0.0f, 0.0f, -1.0f, 17, 14, 23, 20);
  builder.SetNormal(0.0f, -1.0f, 0.0f, 8, 11, 19, 21);
  builder.SetNormal(-1.0f, 0.0f, 0.0f, 13, 1, 18, 7);
  builder.SetUv(0.0f, 1.0f, 0, 5, 17, 13, 8, 12);
  builder.SetUv(1.0f, 1.0f, 3, 16, 14, 11, 15, 1);
  builder.SetUv(0.0f, 0.0f, 6, 10, 23, 19, 2, 18);
  builder.SetUv(1.0f, 0.0f, 9, 22, 20, 21, 4, 7);
  builder.SetTriangle(0, 6, 3);
  builder.SetTriangle(3, 6, 9);
  builder.SetTriangle(5, 10, 16);
  builder.SetTriangle(16, 10, 22);
  builder.SetTriangle(17, 23, 14);
  builder.SetTriangle(14, 23, 20);
  builder.SetTriangle(8, 19, 11);
  builder.SetTriangle(11, 19, 21);
  builder.SetTriangle(13, 18, 1);
  builder.SetTriangle(1, 18, 7);
  builder.SetTriangle(12, 2, 15);
  builder.SetTriangle(15, 2, 4);
  return std::shared_ptr<Shape>(builder.shape);
}

std::shared_ptr<Shape> ShapeBuilder::Quad(float width, float height, unsigned int subdivisions) {
  ShapeBuilder builder(4, 2);
  float x = abs(width / 2.0f);
  float y = abs(height / 2.0f);
  builder.SetVertex(-x, +y, 0.0f); // 0------2
  builder.SetVertex(-x, -y, 0.0f); // |      |
  builder.SetVertex(+x, +y, 0.0f); // |      |
  builder.SetVertex(+x, -y, 0.0f); // 1------3
  builder.SetNormal(0.0f, 0.0f, 1.0f, 0, 1, 2, 3);
  builder.SetUv(0.0f, 1.0f);
  builder.SetUv(0.0f, 0.0f);
  builder.SetUv(1.0f, 1.0f);
  builder.SetUv(1.0f, 0.0f);
  builder.SetTriangle(0, 1, 2);
  builder.SetTriangle(2, 1, 3);
  return std::shared_ptr<Shape>(builder.shape);
}

std::shared_ptr<Shape> ShapeBuilder::Sphere() {
  const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;
  ShapeBuilder builder(12, 19);
  builder.SetVertex(-1.0f, +t,    +0.0);
  builder.SetVertex(+1.0f, +t,    +0.0);
  builder.SetVertex(-1.0f, -t,    +0.0);
  builder.SetVertex(+1.0f, -t,    +0.0);
  builder.SetVertex(+0.0f, -1.0f, +t);
  builder.SetVertex(+0.0f, +1.0f, +t);
  builder.SetVertex(+0.0f, -1.0f, -t);
  builder.SetVertex(+0.0f, +1.0f, -t);
  builder.SetVertex(+t,    +0.0f, -1.0);
  builder.SetVertex(+t,    +0.0f, +1.0);
  builder.SetVertex(-t,    +0.0f, -1.0);
  builder.SetVertex(-t,    +0.0f, +1.0);
  builder.SetTriangle(0, 11, 5);
  builder.SetTriangle(0, 5, 1);
  builder.SetTriangle(0, 1, 7);
  builder.SetTriangle(0, 7, 10);
  builder.SetTriangle(0, 10, 11);
  builder.SetTriangle(1, 5, 9);
  builder.SetTriangle(5, 11, 4);
  builder.SetTriangle(11, 10, 2);
  builder.SetTriangle(10, 7, 6);
  builder.SetTriangle(7, 1, 8);
  builder.SetTriangle(3, 9, 4);
  builder.SetTriangle(3, 4, 2);
  builder.SetTriangle(3, 2, 6);
  builder.SetTriangle(3, 6, 8);
  builder.SetTriangle(3, 8, 9);
  builder.SetTriangle(4, 9, 5);
  builder.SetTriangle(2, 4, 11);
  builder.SetTriangle(6, 2, 10);
  builder.SetTriangle(8, 6, 7);
  builder.SetTriangle(9, 8, 1);
  return std::shared_ptr<Shape>(builder.shape);
}

/*

	// Vertices

	// Faces

 */
