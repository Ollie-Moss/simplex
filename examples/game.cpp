#include "core/Registry.h"
#include "core/Scene.h"
#include "core/Simplex.h"
#include "graphics/text/Font.h"
#include "gui/components/DebugElement.h"
#include "gui/systems/UILayoutSystem.h"
#include "gui/systems/UIRenderSystem.h"
#include "gui/utility/UISpecification.h"
#include "systems/RenderSystem.h"

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

        DebugElement().Build(m_Registry);
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
