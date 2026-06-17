#pragma once

#include "core/Types.h"
#include <string>

struct UIElement
{
    std::string id = "N/A";
    EntityId parent = NULL_ENTITY;
    std::vector<EntityId> children;
    bool toBeDeleted = false;
    bool dirty = true;
};
