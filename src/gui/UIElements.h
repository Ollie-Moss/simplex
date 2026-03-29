#pragma once

#include "UIBuilderTypes.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "gui/Bindable.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"
#include "gui/UILayoutHelpers.h"
#include "gui/UILayoutTypes.h"
#include <format>
#include <functional>
#include <ostream>

template <typename... TComponents>
inline ElementHandle Element(DefaultUIProps props, TComponents &...extra)
{
    return ElementHandle(props, std::forward<TComponents>(extra)...);
}

//---------------------------=
// Text
//---------------------------=

inline ElementHandle TextElement(Text text, DefaultUIProps props = {})
{
    props.text = text;
    return Element(props);
}

inline ElementHandle TextElement(std::string content, Color color = BLACK, DefaultUIProps props = {})
{
    Text text = {.content = content, .color = color};
    return TextElement(text, props);
}

inline ElementHandle TextElement(BindingFunc<std::string> bind, Color color = BLACK, DefaultUIProps props = {})
{
    Text text = {.content = Bind<std::string>(bind), .color = color};
    return TextElement(text, props);
}

//---------------------------=
// Drop Down
//---------------------------=
inline ElementHandle DropDown(BindingFunc<std::string> title = [] { return ""; }, std::function<void()> childrenFn = [] {})
{
    return Element({
                       .layout = {
                           .direction = Direction::Vertical,
                           .padding = 10.0f,
                       },
                       .style = {.color = BLUE},
                   })
        .Children([&] {
            TextElement(title, BLACK, {
                                          .layout = {.sizing = Sizing(HUG, HUG), .direction = Direction::Vertical, .gap = 10.0f},
                                          .style = {.color = RED},
                                          .events{
                                              .onClick = [](ClickEvent e, Entity entity) {
                                                  auto [elem] = entity.GetComponents<UIElement>();
                                                  Entity container = elem.children.front();
                                                  auto &layout = container.GetComponent<UILayout>();
                                                  layout.sizing.Get()
                                                      .height.Set(layout.sizing.Get().height.Get().mode == SizingMode::Hug ? Axis(SizingMode::Fixed, Pixels(10.0f)) : HUG);
                                              },
                                          },
                                      })
                .Children([&] {
                    Element({
                                .layout = {
                                    .sizing = Sizing{
                                        .height = Axis(SizingMode::Fixed, Pixels(10.0f)),
                                    },
                                    .direction = Direction::Vertical,
                                    .gap = 10.0f,
                                },
                            })
                        .Children(childrenFn);
                });
        });
}

//---------------------------=
// Input
//---------------------------=

inline ElementHandle InputElement(UIInput input, DefaultUIProps props = {})
{
    props.layout = {.sizing = Sizing{.width = GROW, .height = GROW}, .padding = 10.0f};
    props.style = {.color = GREEN};

    return Element<UIInput>(props, input);
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

inline ElementHandle SIMPLEX__DEBUG_STATS()
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
        });
}

inline ElementHandle SIMPLEX__DEBUG_ELEMENT(Entity elementEntity)
{
    return DropDown([&] { return std::format("Element: {}", (int)elementEntity); }, [&] {

        EntityId entityId = (EntityId)elementEntity;

        TextElement([entityId] {
            auto ui_element = Simplex::GetRegistry().GetComponent<UIElement>(entityId);
            return std::format("Id: {}", ui_element.id);
        });
        TextElement([entityId] {
            auto ui_element = Simplex::GetRegistry().GetComponent<UIElement>(entityId);
            return std::format("Element Dirty: {}", ui_element.dirty);
        });
        TextElement([entityId] {
            auto transform = Simplex::GetRegistry().GetComponent<UITransform>(entityId);
            return std::format("Size: x: {:^10.0f} y: {:^10.0f}", transform.size.x, transform.size.y);
        });
        TextElement([entityId] {
            auto transform = Simplex::GetRegistry().GetComponent<UITransform>(entityId);
            return std::format("Position: x: {:^10.0f} y: {:^10.0f}", transform.position.x, transform.position.y);
        });
        TextElement([entityId] {
            auto text = Simplex::GetRegistry().GetComponent<Text>(entityId);
            return std::format("Text: {}", text.content.Get());
        }); });
}

inline ElementHandle DEBUG_TREE(Entity entity)
{
    EntityId id = (EntityId)entity;
    return Element({}).Children([id] {
        SIMPLEX__DEBUG_ELEMENT(id);

        Element({}).BindChildren([id] {
            std::cout << "BINDING CHILDREN ENTITY ID: " << id << "\n";
            auto &elem = Simplex::GetRegistry().GetComponent<UIElement>(id);
            for(EntityId child : elem.children)
            {
                std::cout << "Child: " << child << "\n";
                DEBUG_TREE(child);
            }
        });
    });
}
inline ElementHandle SIMPLEX__DEBUG_UI_TREE(Entity UIRoot)
{
    std::cout << "[ELEMENT HANDLE] Context Size: " << ElementHandle::context.size() << "\n";
    return Element({
                       .id = "DEBUG",
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
            // SIMPLEX__DEBUG_STATS();
            Element({

                        .layout = UILayout{
                            .sizing = Sizing{.width = GROW, .height = HUG},
                            .direction = Direction::Vertical,
                            .padding = Padding(9.0f),
                            .gap = 9.0f,
                            .alignItems = AlignItems::End,
                        },
                        .style = UIStyle{},
                    })
                .Children([&] {
                    DEBUG_TREE(UIRoot);
                });
        });
}
