#include "core/Scene.h"
#include "core/Simplex.h"
#include "graphics/text/Font.h"
#include "gui/utility/ChildSpecificiation.h"
#include "gui/elements/TextElement.h"
#include "gui/systems/UIStateSystem.h"
#include "gui/systems/UIEventSystem.h"
#include "gui/systems/UILayoutSystem.h"
#include "gui/systems/UIRenderSystem.h"
#include "gui/utility/UILayoutHelpers.h"
#include "gui/utility/UISpecification.h"
#include "systems/RenderSystem.h"
#include <format>

int main()
{
    Simplex simplex;

    if(!simplex.Init())
    {
        return 0;
    }
    Simplex::GetAssetManager().Load<Font>("Arial", {.path = "arial.ttf"});

    Scene MainScene = Scene(Simplex::GetModules(), "MainScene", [](Registry &m_Registry) {
        // Systems

        // UI
        // m_Registry.RegisterSystem<UIInputSystem>();
        m_Registry.RegisterSystem<UIStateSystem>();
        m_Registry.RegisterSystem<UILayoutSystem>();

        // Rendering
        m_Registry.RegisterSystem<UIRenderSystem>();
        m_Registry.RegisterSystem<UIEventSystem>();
        // m_Registry.RegisterSystem<UIInputRenderSystem>();
        m_Registry.RegisterSystem<RenderSystem>();

        UISpecification builder;
        builder.Configure(UIElementProperties()
                              .WithLayout({.sizing = Sizing{.width = GROW, .height = GROW}})
                              .WithStyle({.color = BLUE}))
            .Children({
                TextElement([] { return std::format("FPS: {:^10.0f}", Simplex::Get().GetFPS()); }),
            })
            .Build(m_Registry);
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
