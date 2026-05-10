#pragma once

class IUpdater
{
  public:
    IUpdater() = default;
    virtual ~IUpdater() = default;
    virtual bool ShouldUpdate() = 0;

    IUpdater(IUpdater &&) = default;
};
