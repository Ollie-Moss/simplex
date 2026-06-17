#pragma once

#include "core/ITime.h"

class Time : public ITime
{
  public:
    uint32_t GetFrameCount() override { return m_FrameCount; }
    void Tick() override { m_FrameCount++; }

  private:
    uint32_t m_FrameCount = 0;
};
