// #include "core/Scene.h"
// #include "core/Simplex.h"
// #include "graphics/text/Font.h"
// #include "gui/UIElements.h"
// #include "gui/UIEventSystem.h"
// #include "gui/UIInputSystem.h"
// #include "gui/UILayoutSystem.h"
// #include "gui/UIRenderSystem.h"
// #include "gui/UIStateSystem.h"
// #include "systems/RenderSystem.h"
//
// int main()
// {
//     Simplex simplex;
//
//     if(!simplex.Init())
//     {
//         return 0;
//     }
//     Simplex::GetAssetManager().Load<Font>("Arial", {.path = "arial.ttf"});
//
//     Scene MainScene = Scene(Simplex::GetModules(), "MainScene", [](Registry &m_Registry) {
//         // Systems
//
//         // UI
//         m_Registry.RegisterSystem<UIInputSystem>();
//         m_Registry.RegisterSystem<UIStateSystem>();
//         m_Registry.RegisterSystem<UILayoutSystem>();
//
//         // Rendering
//         m_Registry.RegisterSystem<UIRenderSystem>();
//         m_Registry.RegisterSystem<UIEventSystem>();
//         m_Registry.RegisterSystem<UIInputRenderSystem>();
//         m_Registry.RegisterSystem<RenderSystem>();
//     });
//
//     simplex.SetScene(MainScene);
//
//     simplex.Start();
//     return 0;
// }
int main()
{
}
