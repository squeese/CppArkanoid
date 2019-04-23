#ifndef EKSAMENECS17_GAMESTATE_H
#define EKSAMENECS17_GAMESTATE_H

#include <random>
#include <memory>
#include <iostream>
#include "ecs/manager.h"
#include "math/matrix.h"
#include "mesh.h"

struct PlayerComponent : public Component {
  float v = 0.0f;
};

struct VertletPositionComponent : public Component {
  Vec3 prevPosition;
  Vec3 currPosition;
};

struct TransformComponent : public Component {
  Mat4 modelMatrix;
};

struct MeshComponent : public Component {
  Mesh* mesh;
};

/*
struct PositionComponent : public Component {
  Vec3 position;
};

struct VerletPositionComponent : public Component {
  Vec3 prevPosition;
  Vec3 currPosition;
};

struct VertletStickComponent : public Component {
  std::shared_ptr<EntityHandle<VerletPositionComponent>> entity;
};
*/


class GameState {
public:
  ArrayMesh<MeshBufferTypes::UV, MeshBufferTypes::NORMAL, MeshBufferTypes::ELEMENTS> meshBoxRound;
  ArrayMesh<MeshBufferTypes::UV, MeshBufferTypes::NORMAL, MeshBufferTypes::ELEMENTS> meshBoxFlat;
  ArrayMesh<MeshBufferTypes::UV, MeshBufferTypes::NORMAL, MeshBufferTypes::ELEMENTS> meshSphere;

  Mat4 cameraProjMatrix = Mat4::Perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f);
  Mat4 cameraViewMatrix = Mat4::LookAt(Vec3 { 0.0f, 0.0f, -3.0f }, Vec3::ZERO(), Vec3::UP());

  Manager<PlayerComponent, VertletPositionComponent, TransformComponent, MeshComponent> manager;

  std::shared_ptr<System<PlayerComponent, TransformComponent>> playerSystem;
  std::shared_ptr<System<TransformComponent, MeshComponent>> meshRenderSystem;

  GameState() :
    meshBoxRound(ShapeBuilder::BoxRound(1.0f, 1.0f, 1.0f)),
    meshBoxFlat(ShapeBuilder::BoxFlat(1.0f, 1.0f, 1.0f)),
    meshSphere(ShapeBuilder::Sphere()) {

    playerSystem = manager.CreateSystem<PlayerComponent, TransformComponent>();
    meshRenderSystem = manager.CreateSystem<TransformComponent, MeshComponent>();

    // Create Player Component
    std::apply([&](PlayerComponent* pic, TransformComponent* tc, MeshComponent* mc) -> void {
      tc->modelMatrix = Mat4::Translation(0.0f, -1.0f, 0.0f) * Mat4::Scale(1.0f, 0.1f, 0.25f);
      mc->mesh = &meshBoxFlat;
    }, manager.GetEntityComponents(manager.CreateEntity<PlayerComponent, TransformComponent, MeshComponent>()));

    // Create Ball Component
    std::apply([&](VertletPositionComponent* vpc, TransformComponent* tc, MeshComponent* mc) -> void {
      tc->modelMatrix = Mat4::Translation(0.0f, 0.0f, 0.0f) * Mat4::Scale(1.0f, 1.0f, 1.0f);
      mc->mesh = &meshBoxFlat;
    }, manager.GetEntityComponents(manager.CreateEntity<VertletPositionComponent, TransformComponent, MeshComponent>()));

    meshRenderSystem->Update([](TransformComponent* tc, MeshComponent* mc) -> void {
      std::cout << tc << " ";
    });
    std::cout << std::endl;

  }
};

#endif

// Randomizer booyah
/*
std::default_random_engine eng((std::random_device())());
std::uniform_real_distribution<float> fdis(-2.0f, 2.0f);

auto cubes = manager.CreateEntities<TransformComponent, MeshComponent, SpinnerComponent>(64);
std::for_each(cubes.begin(), cubes.end(), [&](const auto& entity) -> void {
  std::apply([&](TransformComponent* t, MeshComponent* m, SpinnerComponent* s) -> void {
    t->mModel = Mat4::Translation(fdis(eng), fdis(eng), fdis(eng));
    s->angles = Vec3 { fdis(eng) / 50.0f, fdis(eng) / 50.0f, fdis(eng) / 50.0f };
  }, manager.GetEntityComponents(entity));
});
 */

/*
spinnerUpdateSystem->Update([&dt](TransformComponent* t, SpinnerComponent* s) -> void {
});
*/


/*
void Update(float deltaTime, float elapsed, const Renderer& renderer) {


  renderer.sceneShader.SetUniformMat4("mM", Mat4::Translation(0.0f, 1.0f, -2.0f) * Mat4::EulerRotationZXY(0.8f, 0.7f, 0.0f));
  glDrawElements(GL_TRIANGLES, renderer.bigBoxRound.Use(), GL_UNSIGNED_SHORT, nullptr);
  glBindVertexArray(0);

  renderer.sceneShader.SetUniformMat4("mM", Mat4::Translation(0.0f, -1.0f, -2.0f) * Mat4::EulerRotationZXY(-0.8f, -0.7f, 0.0f));
  glDrawElements(GL_TRIANGLES, renderer.bigBoxFlat.Use(), GL_UNSIGNED_SHORT, nullptr);
  glBindVertexArray(0);
}
  */

// Render all the cubes spinning
/*
cubeRenderSystem->Update([&](TransformComponent* t, MeshComponent* m) -> void {
  // program.SetUniformMat4("mM", t->mModel);
  // glDrawElements(GL_TRIANGLES, m->mesh->Use(), GL_UNSIGNED_SHORT, nullptr);
});
 */
