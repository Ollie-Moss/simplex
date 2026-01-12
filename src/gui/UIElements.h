#pragma once

#include "UIBuilderTypes.h"
#include "core/Entity.h"
#include "core/Input.h"
#include "core/Types.h"
#include "gui/Bindable.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"
#include "gui/UILayoutHelpers.h"
#include "gui/UILayoutTypes.h"
#include <format>
#include <functional>

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
// Drop Down
//---------------------------=
inline ElementHandle DropDown(std::string title = "", const std::function<void()> &children = [] {})
{
    return TextElement(title).Children([&] {
        Element({
                    .layout = {
                        .sizing = Sizing{
                            .height = Axis(SizingMode::Fixed, Pixels(100.0f)),
                        },
                    },
                    .events{
                        .onClick = [](ClickEvent e, Entity entity) {
                            auto [layout] = entity.GetComponents<UILayout>();
                            layout.sizing.Get().height.Set(GROW);
                        },
                    },
                })
            .Children(children);
    });
}

//---------------------------=
// Input
//---------------------------=

inline ElementHandle InputElement(UIInput input)
{
    return Element<UIInput>({.layout = {.sizing = Sizing{.width = GROW, .height = GROW}}, .style = {.color = GREEN}}, input);
}

//---------------------------=
// Checkbox
//---------------------------=

//---------------------------=
// Slider
//---------------------------=

//---------------------------=
// Helper User Interfaces
//---------------------------=

inline UISpecification SIMPLEX__DEBUG_STATS()
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

inline ElementHandle SIMPLEX__DEBUG_ELEMENT(Entity elementEntity)
{
    return Element({

                       // .layout = UILayout{
                       //     .sizing = Sizing{.width = GROW},
                       //     .direction = Direction::Vertical,
                       //     .padding = Padding(10.0f),
                       //     .gap = 10.0f,
                       //     .alignItems = AlignItems::End,
                       // },
                       // .style = UIStyle{.color = GREEN},
                   })
        .Children([&] {
            auto [elem, transform, layout, textLayout, text] = elementEntity.GetComponents<UIElement, UITransform, UILayout, TextLayout, Text>();

            TextElement([&] { return std::format("Element Dirty: {}", elem.dirty); });
            TextElement([&] { return std::format("Position: x: {} y: {}", transform.position.x, transform.position.y); });
            TextElement([&] { return std::format("Size: x: {} y: {}", transform.size.x, transform.size.y); });
        });
}

inline UISpecification SIMPLEX__DEBUG_UI_TREE(Entity UIRoot)
{
    return Element({
                       .layout = UILayout{
                           .sizing = Sizing{.width = GROW},
                           .direction = Direction::Vertical,
                           .padding = Padding(9.0f),
                           .gap = 9.0f,
                           .alignItems = AlignItems::End,
                       },
                       .style = UIStyle{},
                   })
        .Children([&] {
            Element({

                        .layout = UILayout{
                            .sizing = Sizing{.width = GROW},
                            .direction = Direction::Vertical,
                            .padding = Padding(9.0f),
                            .gap = 9.0f,
                            .alignItems = AlignItems::End,
                        },
                        .style = UIStyle{},
                    })
                .Children([&] {
                    SIMPLEX__DEBUG_ELEMENT(UIRoot);
                    auto [elem] = UIRoot.GetComponents<UIElement>();
                    for(auto child : elem.children)
                    {
                        SIMPLEX__DEBUG_ELEMENT(child);
                    }
                });
        })
        .Take();
}
