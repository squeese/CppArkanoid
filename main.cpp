#include "src/window.h"
#include "src/renderer.h"
#include "src/mesh.h"
#include "src/ecs/manager.h"
#include "src/math/matrix.h"
#include <chrono>

const unsigned int W_WIDTH = 800;
const unsigned int W_HEIGHT = 600;
const float FPS_HZ = 1000.0f / 30.0f;

namespace C {
  struct Player;
  struct VertletPosition;
  struct Transform;
  struct Mesh;
  struct Brick;
  struct Collider;
}

bool IsCollidingBox(const Vec3& apos, const Vec3& bpos, const Vec3& asize, const Vec3& bsize);

int main() {
  // Window manager, SDL and Glew
  Window window(W_WIDTH, W_HEIGHT, "Arknnnoooise!");
  Renderer renderer(W_WIDTH, W_HEIGHT);

  //
  Mat4 cameraProjMatrix = Mat4::Perspective(70.0f, 640.0f/480.0f, 0.1f, 100.0f);
  Mat4 cameraViewMatrix = Mat4::LookAt(Vec3 { 0.0f, 0.0f, -3.0f }, Vec3::ZERO(), Vec3::UP());

  // meshes..
  ArrayMesh<MeshTypes::UV, MeshTypes::NORMAL, MeshTypes::ELEMENTS> meshBoxRound(ShapeBuilder::BoxRound(1.0f, 1.0f, 1.0f));
  ArrayMesh<MeshTypes::UV, MeshTypes::NORMAL, MeshTypes::ELEMENTS> meshBoxFlat(ShapeBuilder::BoxFlat(1.0f, 1.0f, 1.0f));

  // ECS, Entity Component System
  Manager<C::Player, C::VertletPosition, C::Transform, C::Mesh, C::Brick, C::Collider> manager;

  auto meshRenderSystem  = manager.CreateSystem<C::Transform, C::Mesh>();
  auto brickSystem       = manager.CreateSystem<C::Transform, C::Brick, C::Collider>();
  auto transformSystem   = manager.CreateSystem<C::Transform>();
  auto verletSystem      = manager.CreateSystem<C::VertletPosition, C::Transform>();
  // auto collisionSystem   = manager.CreateSystem<C::VertletPosition, C::Transform, C::Brick, C::Collider>();

  // Create Player Component
  auto playerEntity = manager.CreateEntity<C::Player, C::Transform, C::Mesh, C::Collider>();
  playerEntity->Apply([&](auto pic, auto tc, auto mc, auto cc) -> void const {
    tc->position = Vec3 { 0.0f, -1.0f, 0.0f };
    tc->scale = Vec3 { 1.0f, 0.1f, 0.25f };
    cc->size = Vec3 { 1.0f, 0.1f, 0.25f };
  });

  // Create Ball Component
  auto ballEntity = manager.CreateEntity<C::VertletPosition, C::Transform, C::Mesh, C::Collider>();
  ballEntity->Apply([&](auto vpc, auto tc, auto mc, auto cc) -> void {
    tc->position = Vec3 { 0.0f, -0.85f, 0.0f };
    tc->scale = Vec3 { 0.2f, 0.2f, 0.2f };
    vpc->position = tc->position;
    cc->size = Vec3 { 0.2f, 0.2f, 0.2f };
  });

  { // Create the brick components
    unsigned int brickRows = 6;
    unsigned int brickPerRow = 16;
    for (unsigned int i = 0; i < (brickRows * brickPerRow); i++) {
      unsigned int x = i % brickPerRow;
      unsigned int y = i / brickPerRow;
      manager.CreateEntity<C::Transform, C::Brick, C::Collider>()->Apply([&](auto tc, auto bc, auto cc) -> void const {
        float xpos = ((x / (brickPerRow - 1.0f)) - 0.5f) * 3.4f;
        float ypos = ((y / (brickRows - 1.0f)) - 0.5f) * 1.0f + 0.75f;
        tc->position = Vec3 { xpos, ypos, 0.0f };
        tc->scale = Vec3 { 0.15f, 0.15f, 0.15f };
      });
    }
  }

  auto cTime = std::chrono::high_resolution_clock::now();
  double time = 0;
  float sinTime = 0;
  float cosTime = 0;

  do {
    auto nTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> eTime = nTime - cTime;
    float dt = eTime.count() / FPS_HZ;
    cTime = nTime;
    time += eTime.count() * 0.001f;
    sinTime = sin(time);
    cosTime = cos(time);

    // Update the player position
    playerEntity->Apply([&window, &dt, &ballEntity](auto pc, auto tc, auto mc, auto cc) -> void const {
      float x = 0.0f;
      x -= window.KEY_A ? 0.05f : 0.0f;
      x += window.KEY_D ? 0.05f : 0.0f;
      if (x != 0.0f) {
        pc->x = std::clamp(pc->x + x * dt, -1.3f, 1.3f);
        tc->position.x = pc->x;
        tc->update = true;
      }
      if (pc->started) return;
      if (window.KEY_SPACE) {
        pc->started = true;
        ballEntity->Apply([&pc](auto bvpc, auto btc, auto mc, auto cc) -> void const {
          bvpc->position = btc->position - Vec3{ pc->x * 0.1f, 0.04f, 0.0f };
        });
      } else {
        ballEntity->Apply([&pc](auto bvpc, auto btc, auto mc, auto cc) -> void const {
          bvpc->position.x = pc->x;
          btc->position.x = pc->x;
          btc->update = true;
        });
      }
    });

    static const float GAME_WIDTH = 1.8f;
    static const float GAME_HEIGTH = 1.4f;
    verletSystem->Update([&](auto vpc, auto tc) -> void const {
      // Update positions
      Vec3 delta = tc->position - vpc->position;
      vpc->position = tc->position - delta * (1 - dt);
      tc->position += delta * dt;
      tc->update = true;
      // Constrain edges
      if (tc->position.x < -GAME_WIDTH) {
        float dx = tc->position.x - vpc->position.x;
        tc->position.x = -GAME_WIDTH;
        vpc->position.x = -GAME_WIDTH + dx;
      } else if (tc->position.x > GAME_WIDTH) {
        float dx = tc->position.x - vpc->position.x;
        tc->position.x = GAME_WIDTH;
        vpc->position.x = GAME_WIDTH + dx;
      }
      if (tc->position.y > GAME_HEIGTH) {
        float dy = tc->position.y - vpc->position.y;
        tc->position.y = GAME_HEIGTH;
        vpc->position.y = GAME_HEIGTH + dy;
      } else if (tc->position.y < -GAME_HEIGTH) {
        // std::cout << "GAME OVER" << std::endl;
      }
    });

    // update the transforms
    transformSystem->Update([&](auto tc) -> void const {
      if (tc->update) {
        tc->modelMatrix = Mat4::Translation(tc->position) * Mat4::EulerRotationZXY(tc->rotation) * Mat4::Scale(tc->scale);
        tc->update = false;
      }
    });

    // Update the ball (move a light with it)
    ballEntity->Apply([&](auto vpc, auto tc, auto mc, auto cc) -> void const {
      renderer.lineShader.Use();
      renderer.lineShader.SetUniformVec3("lightPosition", tc->position + Vec3 { 0.0f, 0.0f, 1.0f });
      renderer.edgeShader.Use();
      renderer.edgeShader.SetUniformVec3("lightPosition", tc->position + Vec3 { 0.0f, 0.0f, 1.0f });

      // check collision with the player pad
      playerEntity->Apply([&](auto ppc, auto ptc, auto pmc, auto pcc) -> void const {
        if (!ppc->started) return;
        if (IsCollidingBox(tc->position, ptc->position, cc->size, pcc->size)) {
          float dy = tc->position.y - vpc->position.y;
          tc->position.y = ptc->position.y + pcc->size.y / 2 + cc->size.y / 2;
          vpc->position.y = tc->position.y + dy;
        }
      });

      // Check collisions with bricks
      brickSystem->Update([&](Index i, auto ptc, auto pbc, auto pcc) -> void const {
        if (pbc->dead) return;
        if (IsCollidingBox(tc->position, ptc->position, cc->size, pcc->size)) {
          pbc->dead = true;
          float ox = ptc->position.x - tc->position.x;
          float oy = ptc->position.y - tc->position.y;
          // asdf;lasjdflksjadf
          // std::cout << abs(ox) << ", " << abs(oy) << std::endl;

          // well, that was as far as I got..as.dfmsa;lkdfhjaskldfjalsk;jdfkl
          if (abs(ox) < abs(oy)) {
            // std::cout << "x" << std::endl;
            // tc->position.x = -GAME_WIDTH;
            // vpc->position.x = -GAME_WIDTH + dx;
            // float dy = tc->position.y - vpc->position.y;
            // tc->position.y = ptc->position.y - pcc->size.y / 2 - cc->size.y / 2;
            // vpc->position.y = tc->position.y + dy;
          } else {
            float dy = tc->position.y - vpc->position.y;
            if (dy > 0.0f) {
              // iI
              tc->position.y = ptc->position.y + pcc->size.y / 2 - cc->size.y / 2;
              vpc->position.y = tc->position.y + dy;
            } else {
              tc->position.y = ptc->position.y - pcc->size.y / 2 - cc->size.y / 2;
              vpc->position.y = tc->position.y + dy;
            }
          }
        }
      });
    });

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer.BeginFrame();

    renderer.lineShader.Use();
    renderer.lineShader.SetUniformMat4("mP", cameraProjMatrix);
    renderer.lineShader.SetUniformMat4("mV", cameraViewMatrix);
    renderer.lineShader.SetUniformVec3("lightAmbient", Vec3 { 0.5f, 0.5f, 0.5f });
    renderer.lineShader.SetUniformFloat("time", time);

    renderer.edgeShader.Use();
    renderer.edgeShader.SetUniformMat4("mP", cameraProjMatrix);
    renderer.edgeShader.SetUniformMat4("mV", cameraViewMatrix);
    renderer.edgeShader.SetUniformVec3("lightAmbient", Vec3 { 0.5f, 0.5f, 0.5f });

    meshRenderSystem->Update([&](Index i, auto tc, auto mc) -> void {
      renderer.lineShader.Use();
      renderer.lineShader.SetUniformMat4("mM", tc->modelMatrix);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawElements(GL_TRIANGLES, meshBoxFlat.Use(), GL_UNSIGNED_SHORT, nullptr);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_TRIANGLES, meshBoxRound.Use(), GL_UNSIGNED_SHORT, nullptr);

      renderer.edgeShader.Use();
      renderer.edgeShader.SetUniformMat4("mM", tc->modelMatrix);
      glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawElements(GL_TRIANGLES, meshBoxRound.Use(), GL_UNSIGNED_SHORT, nullptr);
    });

    brickSystem->Update([&](auto tc, auto bc, auto cc) -> void const {
      if (bc->dead) return;
      renderer.lineShader.Use();
      renderer.lineShader.SetUniformMat4("mM", tc->modelMatrix);
      glDisable(GL_CULL_FACE);
      glDisable(GL_DEPTH_TEST);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawElements(GL_TRIANGLES, meshBoxFlat.Use(), GL_UNSIGNED_SHORT, nullptr);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_TRIANGLES, meshBoxRound.Use(), GL_UNSIGNED_SHORT, nullptr);

      renderer.edgeShader.Use();
      renderer.edgeShader.SetUniformMat4("mM", tc->modelMatrix);
      glEnable(GL_CULL_FACE);
      glEnable(GL_DEPTH_TEST);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDrawElements(GL_TRIANGLES, meshBoxRound.Use(), GL_UNSIGNED_SHORT, nullptr);
    });

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    renderer.FinalizeFrameBlur();
    renderer.FinalizeFrameBloom();
    // renderer.FinalizeFrameMCBO_0();
    // renderer.FinalizeFrameMCBO_1();
    // renderer.FinalizeFrameBCBO_0();
    // renderer.FinalizeFrameBCBO_1();

  } while(window.HandleInput());

  return 0;
}

namespace C {
  struct Player : public Component {
    bool started = false;
    float x = 0.0f;
  };
  struct VertletPosition : public Component {
    Vec3 position;
  };
  struct Transform : public Component {
    Mat4 modelMatrix;
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    bool update = true;
  };
  struct Mesh : public Component {
    MeshBase* mesh = nullptr;
  };
  struct Brick : public Component {
    Vec3 rotation;
    bool dead = false;
  };
  struct Collider : public Component {
    Vec3 size;
    float radius = 0.2f;
  };
}


bool IsCollidingBox(const Vec3& apos, const Vec3& bpos, const Vec3& asize, const Vec3& bsize) {
  return ((abs(apos.x - bpos.x) * 2 < (asize.x + bsize.x)) &&
          (abs(apos.y - bpos.y) * 2 < (asize.y + bsize.y)) &&
          (abs(apos.z - bpos.z) * 2 < (asize.z + bsize.z)));
}
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
