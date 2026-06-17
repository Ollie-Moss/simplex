#pragma once

#include <cstdint>

class ITime
{
  public:
    virtual ~ITime() = default;
    virtual uint32_t GetFrameCount() = 0;
    virtual void Tick() = 0;
};
