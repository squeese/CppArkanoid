#ifndef EKSAMENECS17_SHAPES_H
#define EKSAMENECS17_SHAPES_H

#include <GL/glew.h>
#include <memory>
#include "math/vec3.h"
#include "math/vec2.h"

struct Triangle {
  GLushort a;
  GLushort b;
  GLushort c;
};

struct Shape {
  ~Shape();
public:
  Vec3* vertices;
  Vec3* normals;
  Vec2* uvs;
  Triangle* triangles;
  GLuint numVertices;
  GLuint numTriangles;
};

class ShapeBuilder {
  Shape* shape;
  unsigned int vertexIndex = 0;
  unsigned int normalIndex = 0;
  unsigned int uvIndex = 0;
  unsigned int triangleIndex = 0;
  void SetVertex(GLfloat x, GLfloat y, GLfloat z);
  template<typename T>
  void SetVertex(GLfloat x, GLfloat y, GLfloat z, T index);
  template<typename T, typename... Args>
  void SetVertex(GLfloat x, GLfloat y, GLfloat z, T index, Args... args);
  void SetNormal(unsigned int a, unsigned int b);
  void SetNormal(unsigned int a, unsigned int b, unsigned c);
  template<typename T>
  void SetNormal(GLfloat x, GLfloat y, GLfloat z, T index);
  template<typename T, typename... Args>
  void SetNormal(GLfloat x, GLfloat y, GLfloat z, T index, Args... args);
  void SetUv(GLfloat x, GLfloat y);
  template<typename T>
  void SetUv(GLfloat x, GLfloat y, T index);
  template<typename T, typename... Args>
  void SetUv(GLfloat x, GLfloat y, T index, Args... args);
  void SetTriangle(GLushort a, GLushort b, GLushort c);
public:
  ShapeBuilder(GLuint numVertices, GLuint numTriangles) : shape(new Shape {
    new Vec3[numVertices],
    new Vec3[numVertices],
    new Vec2[numVertices],
    new Triangle[numTriangles],
    numVertices,
    numTriangles,
  }) {}
  static std::shared_ptr<Shape> BoxRound(float width, float height, float depth);
  static std::shared_ptr<Shape> BoxFlat(float width, float height, float depth);
  static std::shared_ptr<Shape> Quad(float width, float height, unsigned int subdivisions);
};

#endif
