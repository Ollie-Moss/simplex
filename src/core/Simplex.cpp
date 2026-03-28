#include "Simplex.h"
#include "assets/AssetManager.h"
#include "core/Scene.h"
#include "glm/fwd.hpp"
#include "graphics/RendererManager.h"
#include <chrono>
#include <string_view>
#include <sys/types.h>
#include <utility>

// Define at simplex so all other files can just include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Simplex::Simplex(SimplexModules modules)
{
    m_Modules = std::move(modules);
    s_Instance = this;
}

bool Simplex::Init()
{
    if(!m_Modules.m_View->Init("Simplex", 1280, 720))
        return false;

    if(!m_Modules.m_Input->Init(m_Modules.m_View->GetWindow()))
        return false;

    if(!m_Modules.m_RendererManager->Init())
        return false;

    if(!m_Modules.m_AssetManager->Init())
        return false;

    return true;
}

Simplex::~Simplex() {}

void Simplex::SetScene(const Scene &scene)
{
    m_CurrentScene = std::move(scene);
    m_CurrentScene.m_Setup(m_CurrentScene.m_Registry);
}

Simplex &Simplex::Get()
{
    assert(s_Instance);
    return *s_Instance;
}

IView &Simplex::GetView()
{
    return *Get().m_Modules.m_View;
}

IInput &Simplex::GetInput()
{
    return *Get().m_Modules.m_Input;
}

RendererManager &Simplex::GetRendererManager()
{
    return *Get().m_Modules.m_RendererManager;
}

AssetManager &Simplex::GetAssetManager()
{
    return *Get().m_Modules.m_AssetManager;
}

Registry &Simplex::GetRegistry()
{
    return GetScene().m_Registry;
}

Scene &Simplex::GetScene()
{
    return Get().m_CurrentScene;
}

float Simplex::GetFPS()
{
    return m_Fps;
}

float Simplex::GetDeltaTime()
{
    return m_DeltaTime;
}

void Simplex::Start()
{
    GetRegistry().Start();
    Tick();
}

void Simplex::Tick()
{
    using clock = std::chrono::high_resolution_clock;
    const double fixedDelta = 1.0 / 60.0; // 60Hz fixed update

    auto lastTime = clock::now();
    double accumulator = 0.0;

    while(!m_Modules.m_View->ShouldQuit())
    {
        auto now = clock::now();
        std::chrono::duration<double> frameTime = now - lastTime;
        lastTime = now;

        m_DeltaTime = frameTime.count();
        if(m_DeltaTime > 0.25)
            m_DeltaTime = 0.25; // clamp to avoid spiral of death
        accumulator += m_DeltaTime;
        m_Fps = 1.0f / m_DeltaTime;

        m_Modules.m_Input->PollEvents();
        m_Modules.m_View->ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

        GetRegistry().Update(m_DeltaTime);
        // --- Fixed Update Loop ---
        if(accumulator >= fixedDelta)
        {
            GetRegistry().FixedUpdate(accumulator);
            accumulator = 0.0;
        }

        m_Modules.m_RendererManager->Render();

        m_Modules.m_View->SwapBuffers();
    }
}
