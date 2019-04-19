#include "shapes.h"
#include <cmath>

Shape::~Shape() {
  delete vertices;
  delete normals;
  delete triangles;
}

void ShapeBuilder::SetVertex(GLfloat x, GLfloat y, GLfloat z) {
  shape->vertices[vertexIndex++] = Vec3(x, y, z);
}

void ShapeBuilder::SetTriangle(GLushort a, GLushort b, GLushort c) {
  shape->triangles[triangleIndex] = Triangle { a, b, c };
}

std::shared_ptr<Shape> ShapeBuilder::Box(float width, float height, float depth) {
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
  // SetNormal(normals, vertices, 0, 1, 4, 2);
  // SetNormal(normals, vertices, 1, 0, 3, 5);
  // SetNormal(normals, vertices, 2, 0, 3, 6);
  // SetNormal(normals, vertices, 3, 1, 2, 7);
  // SetNormal(normals, vertices, 4, 0, 5, 6);
  // SetNormal(normals, vertices, 5, 1, 4, 7);
  // SetNormal(normals, vertices, 6, 2, 4, 7);
  // SetNormal(normals, vertices, 7, 3, 5, 6);
  return std::shared_ptr<Shape>(builder.shape);
}
