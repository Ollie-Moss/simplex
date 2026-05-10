#pragma once

#include "gui/utility/IUpdater.h"

class AlwaysUpdater : public IUpdater
{
  public:
    bool ShouldUpdate() override
    {
        return true;
    }
};
