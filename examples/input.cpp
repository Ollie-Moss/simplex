#include "core/Registry.h"
#include "core/Scene.h"
#include "core/Simplex.h"
#include "core/SimplexModules.h"
#include "core/Types.h"
#include "graphics/text/Font.h"
#include "gui/utility/ChildSpecificiation.h"
#include "gui/elements/TextElement.h"
#include "gui/systems/UIStateSystem.h"
#include "gui/systems/UIEventSystem.h"
#include "gui/systems/UILayoutSystem.h"
#include "gui/systems/UIRenderSystem.h"
#include "gui/utility/Direction.h"
#include "gui/utility/SizingMode.h"
#include "gui/utility/UIElementProperties.h"
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
        m_Registry.RegisterSystem<UILayoutSystem>();

        // Rendering
        m_Registry.RegisterSystem<UIRenderSystem>();
        m_Registry.RegisterSystem<RenderSystem>();

        UISpecification builder;
        EntityId root = builder.Configure(UIElementProperties()
                                              .WithLayout(UILayoutDefintion().Configure({.sizing = Sizing{.width = Axis(SizingMode::Fixed, Percent(50)), .height = GROW}, .direction = Direction::Vertical}))
                                              .WithStyle(UIStyleDefintion().Configure({.color = BLUE})))
                            .Children({
                                TextElement([](SimplexModules, Registry &) { return std::format("FPS: {:^10}", Simplex::Get().GetFPS()); }),
                                TextElement([](SimplexModules, Registry &) { return std::format("Entities: {}", Simplex::GetRegistry().GetEntityCount()); }),
                                TextElement([](SimplexModules, Registry &) { return std::format("Componentes: {}", Simplex::GetRegistry().GetComponentCount()); }),
                            })
                            .Build(m_Registry);
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
