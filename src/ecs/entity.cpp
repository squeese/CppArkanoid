#include "entity.h"

void EntityHandleManager::PurgeHandle(unsigned long index) {
  if (index == pointers.size() - 1) {
    pointers.pop_back();
    return;
  }
  pointers[index] = pointers.back();
  pointers.pop_back();
  std::shared_ptr<EntityHandleBase> ptr = pointers[index].lock();
  ptr->SetHandleIndex(index);
}
