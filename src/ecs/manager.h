#ifndef EKSAMENECS17_MANAGER_H
#define EKSAMENECS17_MANAGER_H

#include "component.h"
#include "entity.h"
#include "system.h"
#include <vector>

template<typename... MComponents>
class Manager {
  static const unsigned int SIZE;

  std::vector<Entity>                       entities;
  std::vector<std::shared_ptr<SystemBase>>  systems;
  std::vector<uint8_t>                      componentData[SIZE];
  uint32_t                                  componentSizes[SIZE] = { 0 };

  template<size_t index>
  void SetComponentSizes() {}

  template<size_t index, typename TC, typename... REST>
  void SetComponentSizes() {
    componentSizes[index] = sizeof(TC);
    SetComponentSizes<index + 1, REST...>();
  }

  template<size_t offset>
  ComponentMask CreateSystemMask() const {
    return 0;
  }

  template<size_t offset, typename CT, typename... REST>
  ComponentMask CreateSystemMask() const {
    return CreateSystemMask<offset + 1, REST...>() ^ 1UL << ComponentIndex<CT, MComponents...>();
  }

  template <size_t offset, size_t size>
  void CreateSystemComponentKeys(std::array<Index, size>& keys) {}

  template <size_t offset, size_t size, typename CT, typename... REST>
  void CreateSystemComponentKeys(std::array<Index, size>& keys) {
    ComponentIndex<CT, MComponents...> index;
    keys[offset] = (Index) index;
    CreateSystemComponentKeys<offset + 1, size, REST...>(keys);
  }

  template<size_t offset>
  ComponentMask CreateEntityComponents(Entity& entity) const {
    return 0;
  }

  template<size_t offset, typename CT, typename... REST>
  ComponentMask CreateEntityComponents(Entity& entity) {
    ComponentIndex<CT, MComponents...> componentIndex;
    Offset componentOffset = componentData[componentIndex].size();
    entity.componentMap[componentIndex] = componentOffset;
    entity.componentMask ^= 1UL << componentIndex;
    componentData[componentIndex].resize(componentOffset + sizeof(CT));
    auto componentInstance = (CT*) new (&componentData[componentIndex][componentOffset]) CT;
    componentInstance->entityIndex = entity.GetIndex();
    return CreateEntityComponents<offset + 1, REST...>(entity) ^ 1UL << componentIndex;
  }

  template<size_t index>
  std::tuple<> GetEntityData(const Entity& entity) const {
    return std::tuple<>();
  }

  template<size_t index, typename CT, typename...REST>
  std::tuple<CT*, REST*...> GetEntityData(const Entity& entity) const {
    ComponentIndex<CT, MComponents...> componentIndex;
    auto offset = entity.componentMap.find((Index) componentIndex);
    auto componentInstance = (CT*) &componentData[componentIndex][offset->second];
    return std::tuple_cat(std::tuple<CT*>(componentInstance), GetEntityData<index + 1, REST...>(entity));
  }

public:
  Manager() {
    SetComponentSizes<0, MComponents...>();
  }

  auto GetEntityCount() {
    return entities.size();
  }

  template<typename... SComponents>
  std::shared_ptr<System<Manager<MComponents...>, SComponents...>> CreateSystem() {
    using SystemType = System<Manager<MComponents...>, SComponents...>;
    std::array<Index, sizeof...(SComponents)> componentKeys;
    CreateSystemComponentKeys<0, sizeof...(SComponents), SComponents...>(componentKeys);
    auto system = new SystemType { this, CreateSystemMask<0, SComponents...>(), componentKeys };
    std::shared_ptr<SystemType> systemPtr(system);
    systems.emplace_back(systemPtr);
    return systemPtr;
  }

  template<typename... EComponents>
  std::shared_ptr<EntityHandle<Manager<MComponents...>, EComponents...>> CreateEntity() {
    Index index = entities.size();
    auto entityHandle = new EntityHandle<Manager<MComponents...>, EComponents...>(this, index);
    auto entityShared = std::shared_ptr<EntityHandle<Manager<MComponents...>, EComponents...>>(entityHandle);
    entities.emplace_back(index, entityShared);
    CreateEntityComponents<0, EComponents...>(entities[index]);
    for (const auto& system : systems)
      system->PushEntity(entities[index]);
    return entityShared;
  }

  template<typename Component>
  Component* GetComponentInstance(Index index, Offset offset) const {
    return (Component*) &componentData[index][offset];
  }

  template<typename Component>
  Index GetComponentIndex() const {
    return (Index) ComponentIndex<Component, MComponents...> {};
  }

  template<typename... EComponents>
  auto GetEntityComponentData(Index entityIndex) const {
    return GetEntityData<0, EComponents...>(entities[entityIndex]);
  }

  void RemoveEntity(Index entityIndex) {
    for (const auto& system : systems)
      system->PopEntity(entities[entityIndex]);
    Index lastEntityIndex = entities.size() - 1;
    entities[entityIndex] = entities[lastEntityIndex];
    for (const auto& it : entities[entityIndex].componentMap) {
      Index cIndex = it.first;
      Index cOffset = it.second;
      ((Component*) &componentData[cIndex][cOffset])->entityIndex = entityIndex;
    }
    for (const auto& it : entities[lastEntityIndex].componentMap) {
      Index cIndex = it.first;
      Index cOffset = it.second;
      Index lOffset = componentData[cIndex].size() - componentSizes[cIndex];
      if (cOffset != lOffset) {
        memcpy(&componentData[cIndex][cOffset], &componentData[cIndex][lOffset], componentSizes[cIndex]);
        Index eIndex = *(Index *) &componentData[cIndex][cOffset];
        entities[eIndex].componentMap[cIndex] = cOffset;
        for (const auto& system : systems)
          system->UpdateEntity(entities[eIndex]);
      }
      componentData[cIndex].resize(lOffset);
    }
    entities.pop_back();
  }


};

template<typename... SComponents>
const unsigned int Manager<SComponents...>::SIZE(sizeof...(SComponents));

#endif
