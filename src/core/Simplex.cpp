#include "Simplex.h"
#include "core/Scene.h"
#include "glm/fwd.hpp"
#include "graphics/RendererManager.h"
#include "graphics/render-commands/ColliderCommand.h"
#include "graphics/render-commands/TextCommand.h"
#include "graphics/renderers/ColliderRenderer.h"
#include "graphics/renderers/SpriteRenderer.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/renderers/TextRenderer.h"
#include <chrono>
#include <string_view>
#include <sys/types.h>
#include <utility>

Simplex::Simplex()
{
    s_Instance = this;
}

bool Simplex::Init()
{
    if(!m_View.Init("Simplex", 1280, 720))
        return false;

    if(!m_Input.Init())
        return false;

    if(!m_ResourceManager.Init())
        return false;

    m_RendererManager.Register<SpriteCommand, SpriteRenderer>();
    m_RendererManager.Register<TextCommand, TextRenderer>();
    m_RendererManager.Register<ColliderCommand, ColliderRenderer>();
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

View &Simplex::GetView()
{
    return Get().m_View;
}

Input &Simplex::GetInput()
{
    return Get().m_Input;
}

ResourceManager &Simplex::GetResources()
{
    return Get().m_ResourceManager;
}

RendererManager &Simplex::GetRendererManager()
{
    return Get().m_RendererManager;
}

Scene &Simplex::GetScene()
{
    return Get().m_CurrentScene;
}

Registry &Simplex::GetRegistry()
{
    return GetScene().m_Registry;
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

    while(!m_View.ShouldQuit())
    {
        auto now = clock::now();
        std::chrono::duration<double> frameTime = now - lastTime;
        lastTime = now;

        m_DeltaTime = frameTime.count();
        if(m_DeltaTime > 0.25)
            m_DeltaTime = 0.25; // clamp to avoid spiral of death
        accumulator += m_DeltaTime;
        m_Fps = 1.0f / m_DeltaTime;

        m_Input.PollEvents();
        m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

        // --- Fixed Update Loop ---
        if(accumulator >= fixedDelta)
        {
            GetRegistry().FixedUpdate(accumulator);
            accumulator = 0.0;
        }

        GetRegistry().Update(m_DeltaTime);
        m_RendererManager.Render();

        m_View.SwapBuffers();
    }
}
