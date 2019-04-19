#ifndef EKSAMENECS17_ENTITY_H
#define EKSAMENECS17_ENTITY_H

#include <cstdint>
#include <vector>
#include <memory>
#include <functional>

struct Entity {
public:
  uint16_t componentBitmask;
  int32_t next;
  Entity(uint16_t m, int32_t n) : componentBitmask(m), next(n) {}
};

class EntityHandleManager;

class EntityHandleBase {
public:
  virtual ~EntityHandleBase() = default;
  virtual void SetEntityIndex(unsigned long index) = 0;
  virtual void SetHandleIndex(unsigned long index) = 0;
};

template<typename... EntityComponentTypes>
class EntityHandle : public EntityHandleBase {
  unsigned long entityIndex;
  unsigned long handleIndex;
  using ArgsType = std::tuple<EntityComponentTypes*...>;
  using FuncType = std::function<void(EntityComponentTypes*...)>;
public:
  EntityHandleManager* manager;
  EntityHandle(unsigned long ei, unsigned long hi, EntityHandleManager* manager) : entityIndex(ei), handleIndex(hi), manager(manager) {}
  ~EntityHandle() final;
  void SetEntityIndex(unsigned long index) final;
  void SetHandleIndex(unsigned long index) final;
  unsigned long GetEntityIndex() const;
};

class EntityHandleManager {
  std::vector<std::weak_ptr<EntityHandleBase>> pointers;
public:
  template<typename... EntityComponentTypes>
  std::shared_ptr<EntityHandle<EntityComponentTypes...>> CreateHandle(unsigned long entityIndex);
  void PurgeHandle(unsigned long index);
};

template<typename... EntityComponentTypes>
std::shared_ptr<EntityHandle<EntityComponentTypes...>> EntityHandleManager::CreateHandle(unsigned long entityIndex) {
  const auto handle = new EntityHandle<EntityComponentTypes...>(entityIndex, pointers.size(), this);
  std::shared_ptr<EntityHandle<EntityComponentTypes...>> sharedPtr(handle);
  std::weak_ptr<EntityHandle<EntityComponentTypes...>> weakPtr(sharedPtr);
  pointers.push_back(weakPtr);
  return sharedPtr;
}

template<typename... EntityComponentTypes>
void EntityHandle<EntityComponentTypes...>::SetEntityIndex(unsigned long index) {
  entityIndex = index;
};

template<typename... EntityComponentTypes>
void EntityHandle<EntityComponentTypes...>::SetHandleIndex(unsigned long index) {
  handleIndex = index;
};

template<typename... EntityComponentTypes>
unsigned long EntityHandle<EntityComponentTypes...>::GetEntityIndex() const {
  return entityIndex;
};

template<typename... EntityComponentTypes>
EntityHandle<EntityComponentTypes...>::~EntityHandle() {
  manager->PurgeHandle(this->handleIndex);
};

#endif
