#include "entity.h"

void Entity::SetIndex(int index) {
  if (!entityHandle.expired()) {
    SPointer ptr = entityHandle.lock();
    ptr->SetIndex(index);
  }
}
