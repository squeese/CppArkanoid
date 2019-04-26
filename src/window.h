#ifndef EKSAMENECS17_WINDOW_H
#define EKSAMENECS17_WINDOW_H
#define GLM_ENABLE_EXPERIMENTAL

#include <functional>
#include <string>
#include <SDL.h>

class Window {
  SDL_Window* window;
  SDL_GLContext context;
public:
  bool KEY_W = false;
  bool KEY_A = false;
  bool KEY_S = false;
  bool KEY_D = false;
  bool KEY_SPACE = false;

  using RenderFunction = std::function<void(float, float)>;
  Window(int width, int height, const std::string& title);
  bool HandleInput();
  ~Window();
};

#endif
