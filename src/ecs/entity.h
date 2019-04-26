#ifndef EKSAMENECS17_ENTITY_H
#define EKSAMENECS17_ENTITY_H

#include "types.h"
#include <map>
#include <memory>

template<typename... MComponents>
class Manager;
class EntityHandleBase;

class Entity {
  using WPointer = std::weak_ptr<EntityHandleBase>;
  using SPointer = std::shared_ptr<EntityHandleBase>;
  using ComponentMap = std::map<Index, Offset>;

  Index entityIndex;
  WPointer entityHandle;
public:
  ComponentMask componentMask = 0;
  ComponentMap componentMap;

  Entity(Index index, const SPointer& entityHandle) : entityIndex(index), entityHandle(entityHandle) {}

  Entity& operator = (Entity& other) {
    if (&other != this) {
      entityHandle.swap(other.entityHandle);
      componentMap.swap(other.componentMap);
      ComponentMask m = componentMask;
      componentMask = other.componentMask;
      other.componentMask = m;
      SetIndex(entityIndex);
      other.SetIndex(-1);
    }
    return *this;
  }

  void SetIndex(int i);
  Index GetIndex() const {
    return entityIndex;
  }
};

class EntityHandleBase {
  friend void Entity::SetIndex(int i);
  virtual void SetIndex(int index) = 0;
};

template<typename Manager, typename... EComponents>
class EntityHandle : public EntityHandleBase {
  Manager* manager;
  int entityIndex;
  void SetIndex(int index) final {
    entityIndex = index;
  }

public:
  EntityHandle(Manager* manager, Index index) : manager(manager), entityIndex(index) {}

  int GetIndex() {
    return entityIndex;
  }

  void Apply(const std::function<void(Index, EComponents*...)>& fn) {
    std::apply(fn, std::tuple_cat(
      std::tuple<Index>(entityIndex),
      manager->template GetEntityComponentData<EComponents...>(entityIndex)));
  }

  void Apply(const std::function<void(EComponents*...)>& fn) {
    std::apply(fn, manager->template GetEntityComponentData<EComponents...>(entityIndex));
  }
};

#endif
