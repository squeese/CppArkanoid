#include "src/window.h"
#include "src/renderer.h"
#include "src/gamestate.h"

const unsigned int W_WIDTH = 800;
const unsigned int W_HEIGHT = 600;
const float FPS_HZ = 1000.0f / 30.0f;

int main() {
  Window window(W_WIDTH, W_HEIGHT, "Arknnnoooise!");
  Renderer renderer(W_WIDTH, W_HEIGHT);
  GameState gameState;
  auto cTime = std::chrono::high_resolution_clock::now();
  double time = 0;
  float sinTime = 0;
  float cosTime = 0;

  /*
  do {
    auto nTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> eTime = nTime - cTime;
    float dt = eTime.count() / FPS_HZ;
    cTime = nTime;
    time += eTime.count() * 0.001f;
    sinTime = sin(time);
    cosTime = cos(time);

    // Update the player position
    gameState.playerSystem->Update([&window, &dt](PlayerComponent* pc, TransformComponent* tc) -> void const {
      float x = 0.0f;
      x -= window.KEY_A ? 0.1f : 0.0f;
      x += window.KEY_D ? 0.1f : 0.0f;
      x *= dt;
      if (x != 0.0f) {
        // tc->modelMatrix = tc->modelMatrix * Mat4::Translation(x, 0.0f, 0.0f);
        tc->modelMatrix.Translate(x, 0.0f, 0.0f);
      }
    });


    renderer.BeginFrame();
    renderer.sceneShader.Use();
    renderer.sceneShader.SetUniformMat4("mP", gameState.cameraProjMatrix);
    renderer.sceneShader.SetUniformMat4("mV", gameState.cameraViewMatrix);
    renderer.sceneShader.SetUniformVec3("lightAmbient", Vec3 { 0.5f, 0.5f, 0.5f });
    renderer.sceneShader.SetUniformVec3("lightPosition", Vec3 { cosTime * 4.85f, 0.0f, sinTime * 4.85f});

    renderer.sceneShader.SetUniformMat4("mM", Mat4::Identity());
    // glDrawElements(GL_TRIANGLES, gameState.meshBoxFlat.Use(), GL_UNSIGNED_SHORT, nullptr);

    unsigned int i = 0;
    gameState.meshRenderSystem->Update([&i, &renderer, &gameState](TransformComponent* tc, MeshComponent* mc) -> void {
      std::cout << "(" << i << ")" << tc << " ";
      if (i == 0) {
        renderer.sceneShader.SetUniformMat4("mM", Mat4::Translation(0.0f, 1.0f, 0.0f));
        renderer.sceneShader.SetUniformMat4("mM", tc->modelMatrix);
      } else {
        renderer.sceneShader.SetUniformMat4("mM", Mat4::Translation(0.0f, -1.0f, 0.0f));
        renderer.sceneShader.SetUniformMat4("mM", tc->modelMatrix);
      }
      // glDrawElements(GL_TRIANGLES, mc->mesh->Use(), GL_UNSIGNED_SHORT, nullptr);
      glDrawElements(GL_TRIANGLES, gameState.meshBoxFlat.Use(), GL_UNSIGNED_SHORT, nullptr);
      i++;
    });
    std::cout << std::endl;

    renderer.FinalizeFrame();

  } while(window.HandleInput());
   */

  return 0;
}

