#ifndef EKSAMENECS17_MANAGER_H
#define EKSAMENECS17_MANAGER_H

#include <cstdint>
#include <vector>
#include <memory>
#include <functional>
#include "component.h"
#include "entity.h"
#include "system.h"

template <typename T, typename... Ts>
struct Index;

template <typename T, typename... Ts>
struct Index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

template <typename T, typename U, typename... Ts>
struct Index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + Index<T, Ts...>::value> {};

template<typename T, typename... Ts>
constexpr bool contains() { return std::disjunction_v<std::is_same<T, Ts>...>; }

template<typename... AllComponentTypes> class Manager {
  std::vector<uint8_t> components[sizeof...(AllComponentTypes)];
  std::vector<Entity> entities;
  EntityHandleManager entityHandles;

  template<size_t index>
  int32_t CreateBitMask(uint16_t* mask) {
    return -1;
  }

  template<size_t index, typename EntityComponentType, typename... EntityComponentTypes>
  int32_t CreateBitMask(uint16_t* mask) {
    // TODO: make some static assertions about valid components'n stuff, but seems to be WAY above my IQ
    // TODO: Assert - EntityComponentType is in the AllEntityComponents variadic
    // TODO: Assert - No duplicates
    // TODO: Assert - All components are in correct increment order
    // - static_assert(contains<EntityComponentType, AllComponentTypes...>, "UGH! Not valid?!");
    // - static_assert(not contains<EntityComponentType, EntityComponentTypes...>, "UGH! Dupicate??");

    int32_t previousComponentOffset = CreateBitMask<index + 1, EntityComponentTypes...>(mask);
    Index<EntityComponentType, AllComponentTypes...> componentIndex;
    *mask = *mask ^ (uint16_t)(1 << componentIndex);
    auto currentComponentOffset = (int32_t) components[componentIndex].size();
    components[componentIndex].resize(currentComponentOffset + sizeof(EntityComponentType));
    auto instance = new (&components[componentIndex][currentComponentOffset]) EntityComponentType();
    instance->next = previousComponentOffset;
    return currentComponentOffset;
  }

  template<size_t index>
  uint16_t CreateSystemBitMask(const uint16_t mask, std::vector<std::vector<uint8_t>*>& componentsSubset) {
    return mask;
  }

  template<size_t index, typename SystemComponentType, typename... SystemComponentTypes>
  uint16_t CreateSystemBitMask(const uint16_t mask, std::vector<std::vector<uint8_t>*>& componentsSubset) {
    // TODO: make some static assertions about valid components'n stuff, but seems to be WAY above my IQ
    // TODO: Assert - EntityComponentType is in the AllEntityComponents variadic
    // TODO: Assert - No duplicates
    // TODO: Assert - All components are in correct increment order
    Index<SystemComponentType, AllComponentTypes...> componentIndex;
    // int i = (int) componentIndex;
    // std::vector<uint8_t> J = components[i];
    // std::vector<uint8_t>* j = &components[i];
    // componentsSubset[index] = j;
    componentsSubset[index] = &components[componentIndex];
    return CreateSystemBitMask<index + 1, SystemComponentTypes...>(mask, componentsSubset) ^ (uint16_t)(1 << componentIndex);
  }

  template<size_t index>
  std::tuple<> GetEntityComponents(const uint16_t bitmask, const int32_t next, uint16_t offset) {
    return std::tuple<>();
  }

  template<size_t index, typename EntityComponentType, typename... EntityComponentTypes>
  std::tuple<EntityComponentType*, EntityComponentTypes*...> GetEntityComponents(const uint16_t bitmask, const int32_t next, uint16_t offset) {
    while ((bitmask & (1 << offset)) == 0) offset++;
    auto instance = (EntityComponentType*) &components[offset][next];
    std::tuple<std::add_pointer_t<EntityComponentType>> value(instance);
    return std::tuple_cat(value, GetEntityComponents<index + 1, EntityComponentTypes...>(bitmask, instance->next, offset + 1));
  }

public:
  template<typename... EntityComponentTypes>
  std::shared_ptr<EntityHandle<EntityComponentTypes...>> CreateEntity() {
    uint16_t bitmask = 0;
    int32_t offset = CreateBitMask<0, EntityComponentTypes...>(&bitmask);
    entities.emplace_back(bitmask, offset);
    unsigned long entityIndex = entities.size() - 1;
    return entityHandles.CreateHandle<EntityComponentTypes...>(entityIndex);
  }

  template<typename... EntityComponentTypes>
  std::vector<std::shared_ptr<EntityHandle<EntityComponentTypes...>>> CreateEntities(unsigned int count) {
    std::vector<std::shared_ptr<EntityHandle<EntityComponentTypes...>>> entitiesArray;
    for (unsigned int i = 0; i < count; i++) {
      entitiesArray.push_back(CreateEntity<EntityComponentTypes...>());
    }
    return entitiesArray;
  }

  template<typename... EntityComponentTypes>
  std::tuple<EntityComponentTypes*...> GetEntityComponents(const std::shared_ptr<EntityHandle<EntityComponentTypes...>>& handle) {
    const Entity* entity = &entities[handle->GetEntityIndex()];
    return GetEntityComponents<0, EntityComponentTypes...>(entity->componentBitmask, entity->next, 0);
  }

  template<typename... SystemComponentTypes>
  System<SystemComponentTypes...>* CreateSystem() {
    std::vector<std::vector<uint8_t>*> componentsSubset(sizeof...(SystemComponentTypes));
    uint16_t bitmask = CreateSystemBitMask<0, SystemComponentTypes...>(0, componentsSubset);
    auto system = new System<SystemComponentTypes...>(bitmask, componentsSubset);
    for (uint32_t i = 0; i < entities.size(); i++) {
      system->EntityCreated(&entities[i], i);
    }
    return system;
  }
};

#endif
