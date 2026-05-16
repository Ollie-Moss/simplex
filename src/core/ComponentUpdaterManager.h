#pragma once

#include "core/Types.h"
#include "gui/utility/IComponentUpdater.h"
#include <map>
#include <memory>
#include <vector>

class ComponentUpdaterManager
{
  public:
    void AddUpdater(EntityId entityId, std::shared_ptr<IComponentUpdater> updater)
    {
        m_ComponentUpdaters[entityId].push_back(updater);
    }

    void EntityDestroyed(EntityId entityId)
    {
        m_ComponentUpdaters[entityId].clear();
    }

    void Update()
    {
        for(auto &[entity, updaters] : m_ComponentUpdaters)
        {
            for(auto &updater : updaters)
            {
                updater->Update(entity);
            }
        }
    }

  private:
    std::map<EntityId, std::vector<std::shared_ptr<IComponentUpdater>>> m_ComponentUpdaters;
};
