#pragma once

#include "core/Simplex.h"
#include "gui/utility/IUpdater.h"
#include <cstdint>

class FrameUpdater : public IUpdater
{
  public:
    bool ShouldUpdate() override
    {
        bool isNewFrame = Simplex::GetTime().GetFrameCount() > m_LastFrameCount;

        if(isNewFrame)
            m_LastFrameCount = Simplex::GetTime().GetFrameCount();

        return isNewFrame;
    }

  private:
    uint32_t m_LastFrameCount = 0;
};
