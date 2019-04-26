/*
#include <iostream>
#include <random>
#include "src/ecs/manager.h"

struct A : public Component {
  static const std::string Name;
  int v = 0;
  float uno = 0.0f;
  float dos = 0.0f;
  float quattro = 0.0f;
  float zinko = 0.0f;
  float bunk = 0.0f;
  float asdf = 0.0f;
};

struct B : public Component {
  static const std::string Name;
  int v = 0;
};

struct C : public Component {
  static const std::string Name;
  int v = 0;
  double value = 0.0f;
};

struct D : public Component {
  static const std::string Name;
  int v = 0;
  bool value = 0.0f;
};

const std::string A::Name("A");
const std::string B::Name("B");
const std::string C::Name("C");
const std::string D::Name("D");
*/

int main() {
  /*
  Manager<A, B, C, D> manager;
  auto system0 = manager.CreateSystem<A, C>();
  auto system1 = manager.CreateSystem<B, C, D>();
  auto system2 = manager.CreateSystem<A, B, C, D>();
  auto system3 = manager.CreateSystem<C, D>();
  auto system4 = manager.CreateSystem<A, B, C>();
  auto system5 = manager.CreateSystem<A, C, D>();

  auto set0 = [&manager]() -> void const {
    manager.CreateEntity<D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, B, C, D>();
    manager.CreateEntity<B, C, D>();
    manager.CreateEntity<D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, C>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, B, C, D>();
    manager.CreateEntity<D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, B, C, D>();
    manager.CreateEntity<B, C, D>();
    manager.CreateEntity<D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, C>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, B, C, D>();
  };
  auto set1 = [&manager]() -> void const {
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, B, C, D>();
    manager.CreateEntity<A, C>();
    manager.CreateEntity<A, B, C, D>();
    manager.CreateEntity<D>();
    manager.CreateEntity<A, B, C, D>();
  };
  auto set2 = [&manager]() -> void const {
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, B, C, D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<B, C, D>();
    manager.CreateEntity<A, C>();
    manager.CreateEntity<A, B, C, D>();
  };
  auto set3 = [&manager]() -> void const {
    manager.CreateEntity<A, D>();
    manager.CreateEntity<B, C, D>();
    manager.CreateEntity<B, C, D>();
    manager.CreateEntity<D>();
    manager.CreateEntity<A, D>();
    manager.CreateEntity<A, C>();
    manager.CreateEntity<A, B, C, D>();
  };

  std::shared_ptr<EntityHandle<Manager<A, B, C, D>, A, B, C, D>> entity;


  auto chop = [&manager, &entity](unsigned int n) -> void {
    unsigned int cursor = 0;
    for (unsigned int cursor = 0; manager.GetEntityCount() > 1; cursor += n) {
      cursor %= manager.GetEntityCount();
      if (cursor == entity->GetIndex()) {
        std::cout << "skipping " << cursor << std::endl;
        continue;
      }
      manager.RemoveEntity(cursor);
      // std::cout << entity->GetIndex() << std::endl;
    }
    entity->Apply([](Index index, A* a, B* b, C* c, D* d) -> void {
      std::cout << " " << index << " ";
      std::cout << " " << a->v << " ";
      std::cout << " " << b->v << " ";
      std::cout << " " << c->v << " ";
      std::cout << " " << d->v << " " << std::endl;
    });
  };


  for (unsigned int n = 0; n < 32; n++) {
    set0();
    entity = manager.CreateEntity<A, B, C, D>();
    entity->Apply([](Index index, A* a, B* b, C* c, D* d) -> void {
      a->v = 11111;
      b->v = 22222;
      c->v = 33333;
      d->v = 44444;
    });
    for (unsigned int i = 0; i < 10; i++) {
      system0->Update([](Index index, A* a, C* c) -> void const {
        a->v++;
        c->v++;
      });
    }
    chop(5 + n);
    manager.Dump();
    system0->Dump();
    break;
    // chop(2 + n / 2);
    // set2();
    // chop(8 + n);
    // set3();
    // chop(2);
  }

  // manager.Dump();
  // system0->Dump();
  // system1->Dump();
  // system2->Dump();
  // system3->Dump();
  // system4->Dump();
  // system5->Dump();

  // manager.RemoveEntity(2);
  // manager.Dump();
  // manager.RemoveEntity(1);
  // manager.Dump();
  // manager.RemoveEntity(0);
  // manager.Dump();
   */
  return 0;
}

