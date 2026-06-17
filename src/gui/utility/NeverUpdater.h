#pragma once

#include "gui/utility/IUpdater.h"
#include <cassert>

class NeverUpdater : public IUpdater
{
  public:
    bool ShouldUpdate() override
    {
        assert(false && "This binding should be set using the Bind<T>() method or by providing a valid updater");
    }
};
