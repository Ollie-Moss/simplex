#include "Simplex.h"
#include "components/SpriteRenderer.h"
#include "core/Entity.h"
#include "glm/fwd.hpp"
#include "systems/RenderSystem.h"
#include <sys/types.h>

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
Registry &Simplex::GetRegistry() {
    return Get().m_Registry;
}

void Simplex::Start() {
    Tick();
}

class GravitySystem : public System {
  public:
    GravitySystem() {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform>();
    }

    void Update() override {
        for (Entity e : m_Entities) {
            Transform &t = e.GetComponent<Transform>();
            t.position = glm::vec3(t.position.z, t.position.y + 0.0001f, t.position.z);
        }
    }
};

void Simplex::Tick() {
    m_Registry.RegisterSystem<RenderSystem>();

    Transform t = {.position = glm::vec3(-1, -1, 0)};
    SpriteRenderer s = {.texture = "GRASS_TILE_1", .size = glm::vec2(.5, .5), .color = glm::vec4(0, 0, 0, 1)};
    Entity e = m_Registry.Create(t, s);

    m_Registry.AddComponent<Transform>(e, t);
    m_Registry.AddComponent<SpriteRenderer>(e, s);

    while (!m_View.ShouldQuit()) {
        m_Input.PollEvents();

        m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        // Do scene tick here
        m_Registry.Update();

        m_Renderer.Render();

        m_View.SwapBuffers();
    }
}
