#include "window.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, const std::string& title) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);

  glewExperimental = GL_TRUE;
  GLenum status = glewInit();
  if (status != GLEW_OK) {
    std::cerr << "GLEW failed to initialize" << std::endl;
    exit(EXIT_FAILURE);
  }

  // if (!glfwInit()) {
  //   std::cerr << "GLFW failed to initialize" << std::endl;
  //   exit(EXIT_FAILURE);
  // }
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glFrontFace(GL_CCW);
  // glCullFace(GL_BACK);
}

Window::~Window() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  context = nullptr;
  window = nullptr;
}

void Window::RenderLoop(unsigned int fps, const RenderFunction& renderFn) {
  float hz = 1000.0f / (float) fps;
  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!isClosed) {
    auto newTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> elapsedTime = newTime - currentTime;
    float deltaTime = elapsedTime.count() / hz;
    currentTime = newTime;
    renderFn(elapsedTime.count(), deltaTime);
    SDL_GL_SwapWindow(window);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)) {
        isClosed = true;
      }
    }
  }
}
