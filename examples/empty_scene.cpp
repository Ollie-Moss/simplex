#include "core/Scene.h"
#include "core/Simplex.h"
#include "graphics/text/Font.h"

int main()
{
    Simplex simplex;

    if(!simplex.Init())
    {
        return 0;
    }

    Simplex::GetAssetManager().Load<Font>("Arial", {.path = "arial.ttf"});

    Scene MainScene = Scene("MainScene", [](Registry &m_Registry) {
    });

    simplex.SetScene(MainScene);

    simplex.Start();
    return 0;
}
