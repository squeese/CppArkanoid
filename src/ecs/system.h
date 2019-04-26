#ifndef EKSAMENECS17_SYSTEM_H
#define EKSAMENECS17_SYSTEM_H

#include <array>

template<typename... MComponents> class Manager;

class SystemBase {
  virtual void PushEntity(const Entity& entity) = 0;
  virtual void PopEntity(const Entity& entity) = 0;
  virtual void UpdateEntity(const Entity& entity) = 0;
  template<typename... MComponents> friend class Manager;
};


template<typename Manager, typename... SComponents>
class System : public SystemBase {
  static const unsigned int SIZE;

  struct Data {
  public:
    Index entityIndex;
    Index componentOffset[SIZE] = {};
    explicit Data(Index i) : entityIndex(i) {}
  };

  const ComponentMask     componentMask;
  const Manager*          manager;
  std::array<Index, SIZE> componentKeys;
  std::vector<Data>       entityData;
  std::map<Index, Index>  entityEntry;

  template<size_t index>
  std::tuple<> GetEntityComponentData(const Index* componentOffset) const {
    return std::tuple<>();
  }

  template<size_t index, typename CT, typename...REST>
  std::tuple<CT*, REST*...> GetEntityComponentData(const Index* componentOffset) const {
    auto instance = manager->template GetComponentInstance<CT>(componentKeys[index], componentOffset[index]);
    return std::tuple_cat(std::tuple<CT*>(instance), GetEntityComponentData<index + 1, REST...>(componentOffset));
  }

  void PushEntity(const Entity& entity) final {
    if ((componentMask & entity.componentMask) != componentMask) return;
    Index systemIndex = entityData.size();
    Index entityIndex = entity.GetIndex();
    entityData.emplace_back(entityIndex);
    for (unsigned int i = 0; i < SIZE; i++) {
      auto key = entity.componentMap.find(componentKeys[i]);
      entityData[systemIndex].componentOffset[i] = key->second;
    }
    entityEntry[entityIndex] = systemIndex;
  }

  void PopEntity(const Entity& entity) final {
    if ((componentMask & entity.componentMask) != componentMask) return;
    Index entityIndex = entity.GetIndex();
    Index systemIndex = entityEntry[entityIndex];
    Index lastIndex = entityData.size() - 1;
    entityEntry[entityData[lastIndex].entityIndex] = systemIndex;
    entityEntry.erase(entityIndex);
    entityData[systemIndex] = entityData[lastIndex];
    entityData.pop_back();
  }

  void UpdateEntity(const Entity& entity) final {
    if ((componentMask & entity.componentMask) == 0) return;
    const auto& entry = entityEntry.find(entity.GetIndex());
    if (entry == entityEntry.end()) return;
    for (unsigned int i = 0; i < SIZE; i++) {
      auto key = entity.componentMap.find(componentKeys[i]);
      entityData[entry->second].componentOffset[i] = key->second;
    }
  }

public:
  System(Manager* manager, ComponentMask mask, const std::array<Index, SIZE>& arr) : manager(manager), componentMask(mask) {
    std::copy(arr.begin(), arr.end(), componentKeys.begin());
  }

  void Update(const std::function<void(Index entityIndex, SComponents*...)>& fn) {
    for (unsigned int i = 0; i < entityData.size(); i++) {
      std::apply(fn, std::tuple_cat(
        std::tuple<Index>(entityData[i].entityIndex),
        GetEntityComponentData<0, SComponents...>(entityData[i].componentOffset)));
    }
  }

  void Update(const std::function<void(SComponents*...)>& fn) {
    for (unsigned int i = 0; i < entityData.size(); i++) {
      std::apply(fn, GetEntityComponentData<0, SComponents...>(entityData[i].componentOffset));
    }
  }
};

template<typename Manager, typename... SComponents>
const unsigned int System<Manager, SComponents...>::SIZE(sizeof...(SComponents));
#endif
