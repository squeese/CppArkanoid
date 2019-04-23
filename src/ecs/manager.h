#ifndef EKSAMENECS17_MANAGER_H
#define EKSAMENECS17_MANAGER_H

#include <iostream>
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

  std::vector<Antity> antities;

  std::vector<std::shared_ptr<SystemBase>> systems;
  EntityHandleManager entityHandles;

  /*
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
   */

  template<size_t index>
  void CreateEntityComponents(int32_t* mask, const std::map<uint32_t, uint32_t>& offsets) {}

  template<size_t index, typename EntityComponentType, typename... REST>
  void CreateEntityComponents(int32_t* mask, const std::map<uint32_t, uint32_t>& offsets) {
    // Get the index for this component (EntityComponentType in AllComponentTypes)
    Index<EntityComponentType, AllComponentTypes...> componentIndex;
    uint32_t offset = components[componentIndex].size();
    // Resize the components array to add a new component
    components[componentIndex].resize(offset + sizeof(EntityComponentType));
    new (&components[componentIndex][offset]) EntityComponentType();
    // Store the offset to pass back to the entity, and flip the bit represting ths component
    // int the component bitmask
    offsets[componentIndex] = offset;
    *mask = *mask ^ (1 << componentIndex);
    // Continue the traversal
    CreateEntityComponents<index + 1, REST...>(mask, offsets);
  }


public:
  template<typename... EntityComponentTypes>
  std::shared_ptr<EntityHandle<EntityComponentTypes...>> CreateEntity() {
    uint32_t index = antities.size();
    antities.emplace_back(index);
    Antity* antity = &antities[index];
    CreateEntityComponents<0, EntityComponentTypes...>(&antity->mask, antity->offsets);

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
  std::shared_ptr<System<SystemComponentTypes...>> CreateSystem() {
    // Create a subset of the component data the system will use, meaning;
    // if this manager holds data for components A, B, C, D; and the system is 'selecting'
    // f.eks components B, D; we create a pointers to those component data and pass it
    // to the system as we create it.
    std::vector<std::vector<uint8_t>*> componentsSubset(sizeof...(SystemComponentTypes));
    uint16_t bitmask = CreateSystemBitMask<0, SystemComponentTypes...>(0, componentsSubset);
    // Create the system with the component bitmask representing the component selection, aswell
    // as the data for each component.
    auto systemInstance = new System<SystemComponentTypes...>(bitmask, componentsSubset);
    // Create a shared pointer as the 'handle' for the system. We need to use a shared pointer
    // because we wont always be the in control of the system, if the 'user' want to disable a system,
    // its done by removing it from the systems array, thus we loose reference to it.
    std::shared_ptr<System<SystemComponentTypes...>> systemPtr(systemInstance);
    // We pass all entities to the new system to let that one filter out which entities that
    // matches the component selection
    for (uint32_t i = 0; i < entities.size(); i++)
      systemInstance->EntityCreated(&entities[i]);
    systems.push_back(systemPtr);
    return systemPtr;
  }

  template<size_t index>
  void DumpComponentData() {
    std::cout << std::endl;
  }

  template<size_t index, typename ComponentType, typename... REST>
  void DumpComponentData() {
    unsigned int size = sizeof(ComponentType);
    unsigned int count = components[index].size() / size;
    std::cout << "-- " << index << " " << ComponentType::Name << " sizeof(" << size << ") size(" << components[index].size() << ") count("<< count <<")" << std::endl;
    for (unsigned int i = 0; i < count; i++) {
      unsigned int offset = i * size;
      ComponentType* instance = (ComponentType*) &components[index][offset];
      std::cout << "[index: " << i << ", offset: "<< offset << " ] " << instance << " next: " << instance->next << std::endl;
    }
    DumpComponentData<index + 1, REST...>();
  }

  void DumpComponentData() {
    std::cout << "-- Dumping Component Data" << std::endl;
    DumpComponentData<0, AllComponentTypes...>();
  }

  template<size_t index>
  std::string DumpEntityBitmask(int32_t bitmask) {
    return "";
  }

  template<size_t index, typename ComponentType, typename... REST>
  std::string DumpEntityBitmask(int32_t bitmask) {
    if ((bitmask) & (1 << index)) {
      return ComponentType::Name + DumpEntityBitmask<index + 1, REST...>(bitmask);
    } else {
      return "-" + DumpEntityBitmask<index + 1, REST...>(bitmask);
    }
  }

  void DumpEntityData() {
    std::cout << "-- Dumping Entity Data" << std::endl;
    for (unsigned int i = 0; i < entities.size(); i++) {
      std::string components = DumpEntityBitmask<0, AllComponentTypes...>(entities[i].componentBitmask);
      std::cout << "[index: " << i << " ] " << components << " next: " << entities[i].next << std::endl;
    }
    std::cout << std::endl;
  }
};

#endif
