#pragma once

#include "core/Types.h"
#include "gui/utility/UISpecification.h"
#include <functional>
#include <memory>
#include <string>

struct UIElement
{
    std::string id = "N/A";
    EntityId parent = NULL_ENTITY;
    std::vector<EntityId> children;
    std::shared_ptr<std::function<std::vector<UISpecification>()>> childrenSpec;
    bool toBeDeleted = false;
    bool dirty = true;
};
