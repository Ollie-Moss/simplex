#include "core/Simplex.h"
#include "core/Scene.h"
#include "core/Entity.h"

#include "gui/UIBuilder.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"
#include "gui/UIElements.h"
#include "gui/UILayoutHelpers.h"
#include "gui/UILayoutTypes.h"
#include "gui/UISystem.h"

#include "graphics/text/Font.h"

#include <vector>
#include <string>
#include <optional>
#include <format>

#include <GLFW/glfw3.h>

// ------------------------------------------------------------
// Todo data
// ------------------------------------------------------------

struct TodoItem
{
    std::string text;
    bool completed = false;
};

struct TodoState
{
    std::vector<TodoItem> items;
};

// ------------------------------------------------------------
// Todo logic system
// ------------------------------------------------------------

class TodoSystem : public System
{
  public:
    TodoSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<TodoState>();
    }

    void Update(float) override
    {
        for(Entity e : m_Entities)
        {
            auto &state = e.GetComponent<TodoState>();

            // Add new item
            if(Simplex::GetInput().OnKeyPressed(GLFW_KEY_N))
            {
                state.items.push_back({"New Todo Item", false});
            }

            if(state.items.empty())
                continue;
        }
    }
};

// ------------------------------------------------------------
// UI specification
// ------------------------------------------------------------

UISpec TodoUI(Entity stateEntity)
{
    return element(
        {
            .layout = UILayout{
                .sizing = Sizing{.width = GROW, .height = GROW},
                .direction = Direction::Vertical,
                .padding = 16.0f,
                .gap = 12.0f,
                .alignItems = AlignItems::Start,
                .justifyContent = JustifyContent::Start,
            },
            .style = UIStyle{
                .color = Hex(0x1E1E1EFF),
            },
        },
        std::vector<UISpec>({
            // Title
            element({
                .layout = UILayout{
                    .sizing = Sizing{.width = HUG},
                },
                .style = UIStyle{
                    .color = Hex(0xFFFFFFFF),
                },
                .text = Text{
                    .content = "Simplex Todo App",
                },
            }),

            // Todo list
            element({
                        .layout = UILayout{
                            .direction = Direction::Vertical,
                            .gap = 8.0f,
                            .alignItems = AlignItems::Start,
                        },
                    },
                    Bind<std::vector<UISpec>>(stateEntity, [](std::optional<Entity> e) {
                        std::vector<UISpec> rows;
                        auto &state = e->GetComponent<TodoState>();

                        for(size_t i = 0; i < state.items.size(); ++i)
                        {
                            auto &item = state.items[i];

                            rows.push_back(
                                element({
                                    .layout = UILayout{
                                        .sizing = Sizing{.width = HUG, .height = HUG},
                                        .direction = Direction::Horizontal,
                                        .padding = 20.0f,
                                    },
                                    .style = UIStyle{
                                        .color = item.completed ? Hex(0x2E7D32FF)  // completed
                                                                : Hex(0x2A2A2AFF), // normal
                                    },
                                    .text = Text{
                                        .content = std::format("{}{}", (item.completed ? "[x] " : "[ ] "), item.text),
                                        .color = item.completed ? BLACK  // completed
                                                                : WHITE, // normal
                                    },
                                    .events = {
                                        .onClick = [i, &item, &state](const ClickEvent &e, Entity) {
                                            if(e.button == GLFW_MOUSE_BUTTON_2)
                                            {
                                                state.items.erase(state.items.begin() + i);
                                                return;
                                            }
                                            item.completed = !item.completed;
                                        },
                                    },
                                }));
                        }

                        return rows;
                    })),

            // Footer
            element({
                .layout = UILayout{.sizing = Sizing{.width = Axis(SizingMode::Fixed, Percent(20)), .height = HUG}, .padding = 20.0f},
                .style = UIStyle{
                    .color = Hex(0xAAAAAAFF),
                },
                .text = Text{
                    .content = "Left Click = Toggle Completion\nRight Click = Remove Todo\nN = New Todo",
                },
            }),
        }));
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{
    Simplex simplex;

    if(!simplex.Init())
        return 0;

    // Assets
    Simplex::GetAssetManager().Load<Font>(
        "Arial",
        {.path = "arial.ttf"});

    Scene TodoScene = Scene("TodoScene", [](Registry &r) {
        // App logic
        r.RegisterSystem<TodoSystem>();

        // UI systems
        r.RegisterSystem<UIStateSystem>();
        r.RegisterSystem<UILayoutSystem>();
        r.RegisterSystem<UIRenderSystem>();
        r.RegisterSystem<UIEventSystem>();

        // State entity
        Entity todoState = r.Create<TodoState>({{
            {"Learn Simplex UI", false},
            {"Build a Todo App", true},
            {"Refactor Renderer", false},
        }});

        // Build UI
        BuildUI(r, TodoUI(todoState));

        // Optional debug UI
        BuildUI(r, DebugUI());
    });

    simplex.SetScene(TodoScene);
    simplex.Start();
    return 0;
}
