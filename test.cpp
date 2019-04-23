#include <iostream>
#include "src/ecs/manager.h"

struct A : public Component {
  static const std::string Name;
  float uno = 0.0f;
  float dos = 0.0f;
};

struct B : public Component {
  static const std::string Name;
  float quattro = 0.0f;
  float zinko = 0.0f;
  float bunk = 0.0f;
  float asdf = 0.0f;
};

struct C : public Component {
  static const std::string Name;
  double value = 0.0f;
};

struct D : public Component {
  static const std::string Name;
  bool value = 0.0f;
};

const std::string A::Name("A");
const std::string B::Name("B");
const std::string C::Name("C");
const std::string D::Name("D");

int main() {
  Manager<A, B, C, D> manager;
  // std::shared_ptr<System<A, C>> systemUno = manager.CreateSystem<A, C>();
  // std::shared_ptr<System<C, D>> systemDos = manager.CreateSystem<C, D>();

  manager.CreateEntity<C>();
  manager.CreateEntity<A, C, D>();
  // manager.CreateEntity<A, D>();
  // manager.CreateEntity<A, C>();
  // manager.CreateEntity<A, B, D>();
  // manager.CreateEntity<A, B, C, D>();
  // manager.CreateEntity<A, B, C, D>();

  // e.Test<B, C>();

  // manager.DumpComponentData();
  // manager.DumpEntityData();
  // systemDos->DumpSourceData();
  // systemDos->DumpComponentData();

  std::cout << std::endl;

  /*
  systemUno->Update([](A* a, C* c) -> void {
    std::cout << a << " ";
  });
  std::cout << std::endl;

  systemDos->Update([](C* c, D* d) -> void {
    std::cout << d << " ";
  });
  std::cout << std::endl;
  */

  return 0;
}

