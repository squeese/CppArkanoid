#ifndef EKSAMENECS17_SYSTEM_H
#define EKSAMENECS17_SYSTEM_H

#include <vector>
#include <array>
#include <functional>
#include "entity.h"

class SystemBase {
public:
  virtual void EntityCreated(const Entity* entity) = 0;
  virtual bool EntityIsCompatible(uint16_t entityBitmask) = 0;
};

template<typename... SystemComponentTypes>
class System : public SystemBase {
  static const uint32_t stride;
  uint16_t componentBitmask;
  std::vector<uint8_t>* componentSource[stride];
  std::vector<std::array<uint32_t, stride>> componentData;

  using FuncType = std::function<void(SystemComponentTypes*...)>;

  template<size_t index>
  void GetEntityComponents(const uint16_t bitmask, const int32_t next, uint16_t offset) {}

  template<size_t index, typename SystemComponentType, typename... REST>
  void GetEntityComponents(const uint16_t bitmask, const int32_t next, uint16_t offset) {
    while ((bitmask & (1 << offset)) == 0) offset++;
    componentData[componentData.size() - 1][index] = next;
    auto instance = (SystemComponentType*) &(*componentSource[index])[next];
    GetEntityComponents<index + 1, REST...>(bitmask, instance->next, offset + (uint16_t) 1);
  }

public:
  System(uint16_t bitmask, const std::vector<std::vector<uint8_t>*>& components) : componentBitmask(bitmask) {
    std::copy(components.begin(), components.end(), componentSource);
  }

  bool EntityIsCompatible(uint16_t entityBitmask) override {
    return false;
  }

  void EntityCreated(const Entity* entity) final {
    if ((componentBitmask & entity->componentBitmask) != componentBitmask) {
      return;
    }
    componentData.resize(componentData.size() + 1);
    GetEntityComponents<0, SystemComponentTypes...>(entity->componentBitmask, entity->next, 0);
  }

  /*
  template<size_t index>
  void Walk(uint32_t next) {}
  template<size_t index, typename ECType, typename... ECRest, typename SCType, typename... SCRest>
  void Walk(uint32_t next) {
    if (typeid(ECType) == typeid(SCType)) {
      std::cout << "MATCH: " << index << std::endl;
      Walk<index + 1, ECRest..., SCRest...>(next);
    } else {
      Walk<index, ECRest..., SCType, SCRest...>(next);
    }
  }
  template<typename... EntityComponentTypes>
  void EntityAppend(uint32_t next) override {
    Walk<0, EntityComponentTypes..., SystemComponentTypes...>(next);
  }
  */

  template<size_t index>
  std::tuple<> GetComponentData(const std::array<uint32_t, stride>& row) {
    return std::tuple<>();
  }

  template<size_t index, typename SystemComponentType, typename...REST>
  std::tuple<SystemComponentType*, REST*...> GetComponentData(const std::array<uint32_t, stride>& row) {
    auto instance = (SystemComponentType*) &(*componentSource[index])[row[index]];
    std::tuple<SystemComponentType*> value(instance);
    return std::tuple_cat(value, GetComponentData<index + 1, REST...>(row));
  }

  void Update(const FuncType& fn) {
    for (const auto& row : componentData) {
      std::apply(fn, GetComponentData<0, SystemComponentTypes...>(row));
    }
  }

  template<size_t index>
  std::string DumpComponentNames() {
    return "";
  }

  template<size_t index, typename ComponentType, typename... REST>
  std::string DumpComponentNames() {
    return ComponentType::Name + DumpComponentNames<index + 1, REST...>();
  }

  template<size_t index>
  void DumpSourceData() {
    std::cout << std::endl;
  }

  template<size_t index, typename ComponentType, typename... REST>
  void DumpSourceData() {
    unsigned int size = sizeof(ComponentType);
    unsigned int count = componentSource[index]->size() / size;
    std::cout << "-- " << index << " " << ComponentType::Name << " sizeof(" << size << ") size(" << componentSource[index]->size() << ") count("<< count <<")" << std::endl;
    for (unsigned int i = 0; i < count; i++) {
      unsigned int offset = i * size;
      ComponentType* instance = (ComponentType*) &((*componentSource[index])[offset]);
      std::cout << "[index: " << i << ", offset: "<< offset << " ] " << instance << " next: " << instance->next << std::endl;
    }
    DumpSourceData<index + 1, REST...>();
  }

  void DumpSourceData() {
    std::string componentNames = DumpComponentNames<0, SystemComponentTypes...>();
    std::cout << "-- Dumping System [ " << componentNames <<  " ] Source Data" << std::endl;
    DumpSourceData<0, SystemComponentTypes...>();
  }

  template<size_t index>
  void DumpComponentData(const std::array<uint32_t, stride>& row) {
    std::cout << std::endl;
  }

  template<size_t index, typename SystemComponentType, typename...REST>
  void DumpComponentData(const std::array<uint32_t, stride>& row) {
    unsigned int ri = row[index];
    auto instance = (SystemComponentType*) &((*componentSource[index])[ri]);
    std::cout << "[" << index << " " << SystemComponentType::Name << " " << ri << " " << instance << " next: ??] ";
    DumpComponentData<index + 1, REST...>(row);
  }

  void DumpComponentData() {
    std::string componentNames = DumpComponentNames<0, SystemComponentTypes...>();
    std::cout << "-- Dumping System [ " << componentNames <<  " ] Component Data" << std::endl;
    for (unsigned int i = 0; i < componentData.size(); i++) {
      const std::array<uint32_t, stride>& row = componentData[i];
      std::cout << "[index: " << i << "] ";
      DumpComponentData<0, SystemComponentTypes...>(row);
    }
    // DumpComponentData<0, SystemComponentTypes...>();
  }
};

template<typename... SystemComponentTypes>
const uint32_t System<SystemComponentTypes...>::stride(sizeof...(SystemComponentTypes));

#endif
