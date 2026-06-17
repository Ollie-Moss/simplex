#pragma once

#include "core/Types.h"
#include "glm/fwd.hpp"
#include "gui/elements/TextElement.h"
#include "gui/utility/UIElementProperties.h"
#include "gui/utility/UILayoutHelpers.h"
#include "gui/utility/UISpecification.h"
#include <format>

inline UISpecification DebugElement()
{
    UISpecification builder;
    return builder.Configure(UIElementProperties()
                                 .WithLayout(UILayoutDefintion().Configure({.sizing = Sizing{.width = HUG, .height = HUG}, .direction = Direction::Vertical, .padding = Padding(10)}))
                                 .WithStyle(UIStyleDefintion().Configure({.color = glm::vec4(0.251, 0.306, 0.341, 1.000)})))
        .Children({
            TextElement([](SimplexModules, Registry &) { return std::format("FPS: {:^10}", Simplex::Get().GetFPS()); }, WHITE),
            TextElement([](SimplexModules, Registry &) { return std::format("Entities: {}", Simplex::GetRegistry().GetEntityCount()); }, WHITE),
            TextElement([](SimplexModules, Registry &) { return std::format("Componentes: {}", Simplex::GetRegistry().GetComponentCount()); }, WHITE),
        });
}
