#pragma once

#include "UIBuilderTypes.h"
#include "UIBuilder.h"
#include "core/Types.h"
#include "gui/Bindable.h"
#include "gui/UIComponents.h"
#include "gui/UILayoutHelpers.h"
#include "gui/UILayoutTypes.h"
#include <format>
#include <optional>

inline UISpecification TextElement(Text text)
{
    return element({.text = text});
}

inline UISpecification TextElement(std::string content, Color color = BLACK)
{
    Text text = {.content = content, .color = color};
    return TextElement(text);
}
inline UISpecification TextElement(BindingFunc<std::string> bind, Color color = BLACK)
{
    Text text = {.content = Bind<std::string>(bind), .color = color};
    return TextElement(text);
}

inline UISpecification DebugUI()
{
    return element(
        {
            .layout = {
                .sizing = Sizing{.width = GROW},
                .direction = Direction::Vertical,
                .padding = Padding(10.0f),
                .gap = 10.0f,
                .alignItems = AlignItems::End,
            },
            .style = UIStyle{},
        },
        [](UISpecification &self) {
            self.AddChild(TextElement([](std::optional<Entity>) {
                return std::format("FPS: {:^10.0f}", Simplex::Get().GetFPS());
            }));

            self.AddChild(TextElement([](std::optional<Entity>) {
                return std::format("Entities: {}", Simplex::GetRegistry().GetEntityCount());
            }));

            self.AddChild(TextElement([](std::optional<Entity>) {
                return std::format("Components: {}", Simplex::GetRegistry().GetComponentCount());
            }));
        });
}
