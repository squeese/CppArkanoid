#ifndef EKSAMENECS17_SHAPES_H
#define EKSAMENECS17_SHAPES_H

#include <GL/glew.h>
#include <memory>
#include "math/vec3.h"

struct Triangle {
  GLuint a;
  GLuint b;
  GLuint c;
};

struct Shape {
  ~Shape();
public:
  Vec3* vertices;
  Vec3* normals;
  Triangle* triangles;
  GLuint numVertices;
  GLuint numTriangles;
};

class ShapeBuilder {
  Shape* shape;
  unsigned int vertexIndex = 0;
  unsigned int normalIndex = 0;
  unsigned int triangleIndex = 0;
  void SetVertex(GLfloat x, GLfloat y, GLfloat z);
  void SetTriangle(GLushort a, GLushort b, GLushort c);
public:
  ShapeBuilder(GLuint numVertices, GLuint numTriangles) : shape(new Shape {
    new Vec3[numVertices],
    new Vec3[numVertices],
    new Triangle[numTriangles],
    numVertices,
    numTriangles,
  }) {}
  static std::shared_ptr<Shape> Box(float width, float height, float depth);
};

#endif

/*
  static Shape CreatePyramid(float radius) { // , unsigned int subDivisions
    GLuint numVertices = 6;
    GLuint numIndices =  8;
float r = abs(radius / 2.0f);
auto vertices = new GLfloat[numVertices * 3];
auto indices = new GLushort[numIndices * 3];
unsigned int vertexIndex = 0;
unsigned int triangleIndex = 0;
// unsigned int edgeIndex = 0;
Set<GLfloat>(vertices, vertexIndex++, 0.0f, 0.0f, +r);
Set<GLfloat>(vertices, vertexIndex++, 0.0f, +r,   0.0f);
Set<GLfloat>(vertices, vertexIndex++, -r,   0.0f, 0.0f);
Set<GLfloat>(vertices, vertexIndex++, 0.0f, -r,   0.0f);
Set<GLfloat>(vertices, vertexIndex++, +r,   0.0f, 0.0f);
Set<GLfloat>(vertices, vertexIndex++, 0.0f, 0.0f, -r);
Set<GLushort>(indices, triangleIndex++, 0, 1, 2);
Set<GLushort>(indices, triangleIndex++, 0, 2, 3);
Set<GLushort>(indices, triangleIndex++, 0, 3, 4);
Set<GLushort>(indices, triangleIndex++, 0, 4, 1);
Set<GLushort>(indices, triangleIndex++, 5, 1, 4);
Set<GLushort>(indices, triangleIndex++, 5, 4, 3);
Set<GLushort>(indices, triangleIndex++, 5, 3, 2);
Set<GLushort>(indices, triangleIndex++, 5, 2, 1);
for (unsigned int s = 0; s < subDivisions; s++) {
  std::cout << "subdivision: " << (s+1) << std::endl;
  unsigned int triangleEnd = triangleIndex;

  for (unsigned int i = 0; i < triangleEnd; i++) {
    GLushort a = indices[i * 3];
    GLushort b = indices[i * 3 + 1];
    GLushort c = indices[i * 3 + 2];
    vec3 va(vertices[a * 3], vertices[a * 3 + 1], vertices[a * 3 + 2]);
    vec3 vb(vertices[b * 3], vertices[b * 3 + 1], vertices[b * 3 + 2]);
    vec3 vc(vertices[c * 3], vertices[c * 3 + 1], vertices[c * 3 + 2]);
    vec3 middle = (va + vb + vc).Normalized() * r;
    indices[i * 3] = vertexIndex;
    Set<GLushort>(indices, triangleIndex++, vertexIndex, a, b);
    Set<GLushort>(indices, triangleIndex++, vertexIndex, c, a);
    Set<GLfloat>(vertices, vertexIndex++, middle.x, middle.y, middle.z);
  }

  for (unsigned int i = 0 ; i < triangleEnd; i++) {

  }
}
return Shape {
vertices,
indices,
numVertices,
numIndices
};
}
*/
