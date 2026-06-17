#pragma once

#include "core/Types.h"

class IComponentUpdater
{
  public:
    virtual ~IComponentUpdater() = default;
    virtual bool Update(EntityId entity) = 0;
};
