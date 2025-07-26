#include "Simplex.h"
#include "core/Scene.h"
#include "glm/fwd.hpp"
#include <string_view>
#include <sys/types.h>
#include <utility>

Simplex::Simplex() {
    s_Instance = this;
}

bool Simplex::Init() {
    if (!m_View.Init("Window", 1280, 720))
        return false;

    if (!m_Input.Init())
        return false;

    if (!m_ResourceManager.Init())
        return false;

    return true;
}

Simplex::~Simplex() {}

void Simplex::SetScene(const Scene &scene) {
    m_CurrentScene = std::move(scene);
    m_CurrentScene.m_Setup(m_CurrentScene.m_Registry);
}

Simplex &Simplex::Get() {
    assert(s_Instance);
    return *s_Instance;
}

View &Simplex::GetView() {
    return Get().m_View;
}

Input &Simplex::GetInput() {
    return Get().m_Input;
}

ResourceManager &Simplex::GetResources() {
    return Get().m_ResourceManager;
}
Renderer2D &Simplex::GetRenderer() {
    return Get().m_Renderer;
}
Scene &Simplex::GetScene() {
    return Get().m_CurrentScene;
}
Registry &Simplex::GetRegistry() {
    return GetScene().m_Registry;
}

void Simplex::Start() {
    Tick();
}

void Simplex::Tick() {
    while (!m_View.ShouldQuit()) {
        m_Input.PollEvents();
        m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

        GetRegistry().Update();
        m_Renderer.Render();

        m_View.SwapBuffers();
    }
}
