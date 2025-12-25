#pragma once

#include "UIBuilderTypes.h"
#include "core/Input.h"
#include "core/Types.h"
#include "gui/Bindable.h"
#include "gui/UIComponents.h"
#include "gui/UILayoutHelpers.h"
#include <format>

template <typename... TComponents>
inline ElementHandle Element(DefaultUIProps props, TComponents &...extra)
{
    return ElementHandle(props, std::forward<TComponents>(extra)...);
}

//---------------------------=
// Text
//---------------------------=

inline ElementHandle TextElement(Text text)
{
    return Element({.text = text});
}

inline ElementHandle TextElement(std::string content, Color color = BLACK)
{
    Text text = {.content = content, .color = color};
    return TextElement(text);
}
inline ElementHandle TextElement(BindingFunc<std::string> bind, Color color = BLACK)
{
    Text text = {.content = Bind<std::string>(bind), .color = color};
    return TextElement(text);
}

//---------------------------=
// Input
//---------------------------=

//---------------------------=
// Checkbox
//---------------------------=

//---------------------------=
// Slider
//---------------------------=

//---------------------------=
// Helper User Interfaces
//---------------------------=
inline UISpecification DebugUI()
{
    return Element({
                       .layout = UILayout{
                           .sizing = Sizing{.width = GROW},
                           .direction = Direction::Vertical,
                           .padding = Padding(10.0f),
                           .gap = 10.0f,
                           .alignItems = AlignItems::End,
                       },
                       .style = UIStyle{},
                   })
        .Children([] {
            TextElement([] {
                return std::format("FPS: {:^10.0f}", Simplex::Get().GetFPS());
            });

            TextElement([] {
                return std::format("Entities: {}", Simplex::GetRegistry().GetEntityCount());
            });

            TextElement([] {
                return std::format("Components: {}", Simplex::GetRegistry().GetComponentCount());
            });
        })
        .Take();
}
