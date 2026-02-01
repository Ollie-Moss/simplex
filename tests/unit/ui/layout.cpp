#include "catch2/catch_test_macros.hpp"
#include "core/Registry.h"
#include "core/Simplex.h"
#include "glm/fwd.hpp"
#include "gui/UILayoutHelpers.h"
#include "gui/UILayoutSystem.h"

TEST_CASE("Single UI Element grows to screen size")
{
    Registry registry;
    registry.RegisterSystem<UILayoutSystem>();

    EntityId entity = registry.Create<UIElement, UITransform, UILayout, Text>({}, {}, {.sizing = Sizing{.width = GROW, .height = GROW}},
                                                                              {});
    registry.Update(0);

    auto transform = registry.GetComponent<UITransform>(entity);
}
