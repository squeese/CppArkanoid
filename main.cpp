#define GLM_ENABLE_EXPERIMENTAL
#include "Graphics/Display.h"
#include "Graphics/Shader.h"
#include "Graphics/Primitives.h"
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


  /*
  std::for_each(cubes.begin(), cubes.end(), [](Cube* c, Transform* t) -> void {
    std::cout << "??" << std::endl;
  });
   */

  /*
  auto cubeEntity = ecs.CreateEntity<Cube, Transform>();
  std::apply([](CubeMeshComponent* cmc, TransformComponent* trc) -> void {
  }, ecs.GetEntityComponents(cubeEntity));
   */

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
  StaticMesh tri(std::move(Primitives::CreatePyramid(0.5f)), matrices, numStuff);

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


// Triangle Mesh
/*
      display.Clear();
      fbo.Capture([&]() -> void {
        cube.Draw(mProj, mView, matrix::EulerRotationZXY(0.0f, 0.0f, 0.0f) * matrix::Translation(-0.5f, -0.5f, -0.5f));
        grid.Draw(mProj, mView, matrix::EulerRotationZXY(0.2f, 0.7f, 0.0f) * matrix::Translation(0.0f, -0.55f, 0.0f));
      });
      display.Update();

    Mesh cube { sizeof(CUBE_POINTS) / sizeof(GLfloat), sizeof(CUBE_INDICES) / sizeof(GLuint), CUBE_POINTS, CUBE_INDICES };
    Plane grid { 32, 0.25f };
    FrameBuffer fbo;



Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0)),
                      Vertex(glm::vec3( 0.0,  0.5, 0)),
                      Vertex(glm::vec3( 0.5, -0.5, 0))};
Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
Shader meshShader("./../Shaders/basic");
*/

// Shader geometryShader(GeometryVertexShaderSource, GeometryFragmentShaderSource);
/*
vec3 points[] = {
  vec3{-0.45f, 0.45f, 0.0f},
  vec3{0.45f, 0.45f, 0.0f},
  vec3{0.45f, -0.45f, 0.0f},
  vec3{-0.45f, -0.45f, 0.0f}
};
// Geometry geometry(points, sizeof(points) / sizeof(points[0]));

matrix projection = matrix::Perspective(70.0f, 640.0f/480.0f, 0.1f, 1000.0f)
        * matrix::LookAt(vec3 { 0.0f, 0.0f, -3.0f }, vec3::ZERO(), vec3::UP());

float i = 0.0f;
while (!display.IsClosed()) {
  i += 0.05f;
  display.Clear();
  // meshShader.Bind();
  matrix modelTransform = projection
          * matrix::Translation(vec3 { sin(i), 0.0f, cos(i) })
          * matrix::EulerRotationZXY(vec3 { 0.0f, sin(i), 0.0f });
  // meshShader.Update(modelTransform);
  // mesh.Draw();
  display.Update();
}
 */

/*
ECS<Position, Rotation, Velocity> ecs;
auto gravity = ecs.CreateSystem<Position, Velocity>();

for (int i = 0; i < 64; i++) {
  auto entity = ecs.CreateEntity<Position, Rotation, Velocity>();
  std::apply([](Position* p, Rotation* r, Velocity* v) -> void {
    p->x = (random() - 0.5f) * 64.0f;
    p->y = (random() - 0.5f) * 64.0f;
    v->x = p->x - 0.1f;
    v->y = p->y + 0.1f;
  }, ecs.GetEntityComponents(entity));
}

using namespace std::chrono;
high_resolution_clock::time_point t1 = high_resolution_clock::now();

for (int i = 0; i < (1024*1024); i++) {
  gravity->Update([](Position* p, Velocity* v) -> void {
    p->x += v->x;
    p->y += v->y;
  });
}

high_resolution_clock::time_point t2 = high_resolution_clock::now();
duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
std::cout << "It took me " << time_span.count() << " seconds.";
std::cout << "It took me " << time_span.count() << " seconds.";
std::cout << std::endl;

// abs->Test();
// std::shared_ptr<EntityHandle<A, C>> e0 = ecs.CreateEntity<A, C>();
// std::apply([](A* a, B* b, C* c) -> void {})
*/
/*
struct Position : Component {
  float x = 0;
  float y = 0;
};

struct Rotation : Component {
  float r = 0;
};

struct Velocity : Component {
  float x = 0;
  float y = 0;
};
*/


/*

class BasicShader : public Shader<2> {
  static const char* VertexShaderSource;
  static const char* FragmentShaderSource;
public:
  BasicShader() : Shader() {
    CompileShader(0, GL_VERTEX_SHADER, VertexShaderSource);
    CompileShader(1, GL_FRAGMENT_SHADER, FragmentShaderSource);
    // Since a fragment shader is allowed to write to multiple buffers, you need to explicitly specify
    // which output is written to which buffer. This needs to happen before linking the program.
    // However, since this is 0 by default and there's only one output right now, the following
    // line of code is not necessary:
    // glBindFragDataLocation(program, 0, "outColor");
    // glAttachShader(program, shaders[0]);
    // glAttachShader(program, shaders[1]);
    // glBindAttribLocation(program, 0, "position");
    LinkAndValidateProgram();
    // uniforms[0] = glGetUniformLocation(program, "transform");
  }
  void Update() final {
    glUseProgram(program);
  }
};

const char* BasicShader::VertexShaderSource(R"glsl(
  #version 150 core
  in vec3 position;
  void main() {
    gl_Position = vec4(position, 1.0);
  }
)glsl");

const char* BasicShader::FragmentShaderSource(R"glsl(
  #version 150 core
  uniform vec3 triangleColor;
  out vec4 outColor;
  void main() {
    outColor = vec4(triangleColor, 1.0);
  }
)glsl");
 */

/*
GLfloat triangle_0_verts[] = {
  -1.0f, -1.0f, +0.7f, 0.7f, 0.9f,
  +0.0f, +0.0f, +0.0f, 0.9f, 0.9f,
  -1.0f, +1.0f, +0.5f, 0.5f, 0.5f,
};
GLushort triangle_0_indices[] = {
  0, 1, 2,
};
GLObject triangle0(3, triangle_0_verts, 1, triangle_0_indices);

GLfloat triangle_1_verts[] = {
  +1.0f, +1.0f, +0.2f, 0.0f, 0.2f,
  +0.0f, +0.0f, +0.0f, 0.9f, 0.9f,
  +1.0f, -1.0f, +0.9f, 0.0f, 0.0f,
};
GLushort triangle_1_indices[] = {
  0, 1, 2,
};
GLObject triangle1(3, triangle_1_verts, 1, triangle_1_indices);

GLfloat vertices[] = {
  -0.5f, +0.5f, 0.0f, 0.5f, 1.0f,
  +0.9f, -0.2f, 1.0f, 0.5f, 0.0f,
  -0.8f, +0.2f, 0.0f, 0.5f, 1.0f,
  +0.4f, -0.7f, 1.0f, 0.5f, 0.0f,
};
GLushort indices[] = {
  0, 1,
  1, 2,
  2, 3
};
GLObject line(4, vertices, 2, indices);
class GLObject {
public:
  GLuint VAO;
  GLuint VBO[2];
  GLuint length;
  GLObject(unsigned int numVertices, GLfloat* vertices, unsigned int numTriangles, GLushort* indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, &VBO[0]);
    glBindVertexArray(VAO);
    // copy vertex data to the gpu
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 5 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    // copy triangle data to the gpu
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTriangles * 3 * sizeof(GLushort), indices, GL_STATIC_DRAW);
    // describe the data for the shaders
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
    // Release etc..
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    length = numTriangles * 3;
  }
  void Draw() {
    glBindVertexArray(VAO);
    // glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_SHORT, nullptr);
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_SHORT, nullptr);
  }
  ~GLObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &VBO[0]);
  }
};
 */
