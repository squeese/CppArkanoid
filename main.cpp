#include "src/window.h"
#include "src/mesh.h"

const unsigned int W_WIDTH = 640;
const unsigned int W_HEIGHT = 480;

int main() {
  Window window(W_WIDTH, W_HEIGHT, "Arknnnoooise!");
  Mesh boxMesh(ShapeBuilder::Box(1.0f, 1.0f, 1.0f));


  window.RenderLoop(30, [](float e, float dt) -> void {

  });
  return 0;
}




/*
#include "src/window.h"
#include "Graphics/Shader.h"
#include "src/shapes.h"
#include "Graphics/Mesh.h"
#include "Math/Matrix.h"
#include "ECS/ECS.h"
#include <vector>
#include <chrono>

const unsigned int numStuff = 128;

struct Camera {
  Matrix projection;
  Matrix view;
};

struct Lights {
  Vector ambient;
};

struct CameraC : public Component {
  float fieldOfView;
  float aspectRatio;
  float near;
  float far;
  Vector position;
  Matrix projection;
  Matrix view;
};

struct Cube : public Component {
};

struct Transform : public Component {
  Vector position;
  Vector rotation;
  Vector scale;
};


int main() {
  ECS<CameraC, Cube, Transform> ecs;
  auto cubeRender = ecs.CreateSystem<Cube, Transform>();

  auto cubes = ecs.CreateEntities<Cube, Transform>(32);
  std::for_each(cubes.begin(), cubes.end(), [&ecs](const auto& entity) -> void {
    std::apply([](Cube* c, Transform* t) -> void {

      std::cout << "?" << std::endl;
    }, ecs.GetEntityComponents(entity));
  });

  Display display(640, 480, "Arknnnoooise!");
  // glEnable(GL_CULL_FACE);
  // glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  Camera camera = {
    Matrix::Perspective(70.0f, 640.0f/480.0f, 0.1f, 1000.0f),
    Matrix::LookAt(Vector { 0.0f, 0.0f, -3.0f }, Vector::ZERO(), Vector::UP()),
  };

  Lights lights = {
    Vector(0.3f, 0.9f, 0.8f),
  };

  GPU_Program<BasicVS, BasicFS> shader;

  Matrix matrices[numStuff];
  for (unsigned int i = 0; i < numStuff; i++) {
    float x = (float)(rand() % 10 - 5) / 5.0f;
    float y = (float)(rand() % 10 - 5) / 5.0f;
    float z = (float)(rand() % 10 - 5) / 5.0f;
    matrices[i] = Matrix::Translation(x, y, z) * Matrix::EulerRotationZXY(x, y, z);
  }
  StaticMesh tri(std::move(shapes::CreatePyramid(0.5f)), matrices, numStuff);

  float second = 0;
  display.RenderLoop(30, [&second, &shader, &tri, &camera, &lights] (float elapsed, float deltaTime) -> void {
      second += elapsed;
      if (second > 1000.0f) {
        std::cout << "(" << elapsed << ") dt: " << deltaTime << std::endl;
        second -= 1000.0f;
      }

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader.Use();
      shader.SetUniformMat4("mP", camera.projection);
      shader.SetUniformMat4("mV", camera.view);
      shader.SetUniformVec3("ambient", lights.ambient);

      // glBindVertexArray(box.vertexArrayObject);
      // glDrawElements(GL_TRIANGLES, box.count, GL_UNSIGNED_SHORT, nullptr);

      // glEnable(GL_CULL_FACE);
      // glEnable(GL_DEPTH_TEST);
      // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      // shader.SetUniformMat4("mM", matrix::EulerRotationZXY(0.0f, x, 0.0f) * matrix::Translation(+0.5f, -0.15f, 0.0f));
      // glBindVertexArray(box.vertexArrayObject);
      // glDrawElements(GL_TRIANGLES, box.count, GL_UNSIGNED_SHORT, nullptr);

      // shader.SetUniformMat4("mM", matrix::EulerRotationZXY(0.0f, x, 0.0f) * matrix::Translation(-0.5f, +0.15f, 0.0f));
      // glBindVertexArray(tri.vertexArrayObject);
      // glDrawElements(GL_TRIANGLES, tri.count, GL_UNSIGNED_SHORT, nullptr);

      glBindVertexArray(tri.vertexArrayObject);
      glDrawElementsInstanced(GL_TRIANGLES, tri.count, GL_UNSIGNED_SHORT, nullptr, numStuff);

      // for (unsigned int i = 0; i < numStuff; i++) {
      //   shader.SetUniformMat4("mModel", matrices[i]); // * matrix::EulerRotationZXY(0.0f, x, 0.0f)
      //   glDrawElements(GL_TRIANGLES, tri.count, GL_UNSIGNED_SHORT, nullptr);
      // }
  });
}
*/
