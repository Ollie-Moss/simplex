#include "Simplex.h"
#include "core/Renderer2D.h"
#include "core/ResourceManager.h"
#include "glm/ext/vector_float4.hpp"
#include "glm/fwd.hpp"

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

void Simplex::Start() {
    Tick();
}

void Simplex::Tick() {
    Renderer2D renderer;
    while (!m_View.ShouldQuit()) {
        m_Input.PollEvents();

        m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        // Do scene tick here
        renderer.Queue({.texture = "GRASS_TILE_1", .postition = glm::vec3(-.5, -.5, 0), .size = glm::vec2(.5, .5), .color = glm::vec4(0, 0, 0, 1)});
        renderer.Render();

        m_View.SwapBuffers();
    }
}
