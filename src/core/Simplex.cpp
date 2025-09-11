#include "Simplex.h"
#include "core/Scene.h"
#include "glm/fwd.hpp"
#include "graphics/QuadRenderer.h"
#include <chrono>
#include <string_view>
#include <sys/types.h>
#include <utility>

Simplex::Simplex()
{
    s_Instance = this;
    m_RendererManager.Register<Quad, QuadRenderer>();
}

bool Simplex::Init()
{
    if(!m_View.Init("Simplex", 1280, 720))
        return false;

    if(!m_Input.Init())
        return false;

    if(!m_ResourceManager.Init())
        return false;

    if(!m_Renderer.Init())
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
Renderer2D &Simplex::GetRenderer()
{
    return Get().m_Renderer;
}
Scene &Simplex::GetScene()
{
    return Get().m_CurrentScene;
}
Registry &Simplex::GetRegistry()
{
    return GetScene().m_Registry;
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

        double dt = frameTime.count();
        if(dt > 0.25)
            dt = 0.25; // clamp to avoid spiral of death
        accumulator += dt;

        m_Input.PollEvents();
        m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

        // --- Fixed Update Loop ---
        if(accumulator >= fixedDelta)
        {
            GetRegistry().FixedUpdate();
            accumulator = 0.0;
        }

        GetRegistry().Update();
        m_Renderer.Render();

        Quad quad = {};
        m_RendererManager.Submit(&quad);

        m_View.SwapBuffers();
    }
}
