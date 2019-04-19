#ifndef EKSAMENECS17_SYSTEM_H
#define EKSAMENECS17_SYSTEM_H

#include <vector>
#include <map>
#include <array>

class SystemBase {};

template<typename... SystemComponentTypes>
class System : public SystemBase {
  static const uint32_t stride;
  uint16_t componentBitmask;
  std::vector<uint8_t>* componentSource[stride];
  std::vector<std::array<uint32_t, stride>> componentData;
  // std::map<uint32_t, uint32_t> entityToComponentMap;
  // using ArgsType = std::tuple<SystemComponentTypes*...>;
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
  void EntityCreated(const Entity* entity, const unsigned long index) {
    if ((componentBitmask & entity->componentBitmask) != componentBitmask) return;
    componentData.resize(componentData.size() + 1);
    GetEntityComponents<0, SystemComponentTypes...>(entity->componentBitmask, entity->next, 0);
  }

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
};



template<typename... SystemComponentTypes>
const uint32_t System<SystemComponentTypes...>::stride(sizeof...(SystemComponentTypes));

#endif
