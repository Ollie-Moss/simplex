#include "core/Scene.h"
#include "core/Simplex.h"
#include "graphics/text/Font.h"
#include "gui/Text.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"
#include "gui/UIElements.h"
#include "gui/UIBuilder.h"
#include "gui/UILayoutHelpers.h"
#include "gui/UILayoutTypes.h"
#include "gui/UISystem.h"
#include "systems/RenderSystem.h"
#include <format>

UISpec UI()
{
    return element({
        .layout = {
            .sizing = Sizing{.width = Axis(SizingMode::Fixed, Percent(50)), .height = GROW},
            .padding = 20.0f,
        },
        .style = {
            .color = BLUE,
        },
        .text = {
            .content = "Hi",
        },
        .events = {
            .onClick = [](const ClickEvent &e, Entity entity) {
                entity.GetComponent<Text>().content = std::format("button: {}\nx: {:.0f}, y: {:.0f}", e.button, e.mousePos.x, e.mousePos.y);
            },
        },
    });
}

int main()
{
    Simplex simplex;

    if(!simplex.Init())
    {
        return 0;
    }
    Simplex::GetAssetManager().Load<Font>("Arial", {.path = "arial.ttf"});

    Scene MainScene = Scene("MainScene", [](Registry &m_Registry) {
        // Systems

        // UI
        m_Registry.RegisterSystem<UIStateSystem>();
        m_Registry.RegisterSystem<UILayoutSystem>();

        // Rendering
        m_Registry.RegisterSystem<UIRenderSystem>();
        m_Registry.RegisterSystem<UIEventSystem>();
        m_Registry.RegisterSystem<RenderSystem>();

        BuildUI(m_Registry, UI());
        BuildUI(m_Registry, DebugUI());
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
