#include "Simplex.h"
#include "core/Registry.h"
#include "core/Renderer2D.h"
#include "core/ResourceManager.h"
#include "glm/fwd.hpp"
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

struct Transform {
    glm::vec3 position;
};

class GravitySystem : public System {
  public:
    GravitySystem() {
        m_Signature = Simplex::GetRegistry().CreateSignature<Transform>();
    }

    void Update() override {
        for (Entity e : m_Entities) {
            Transform t = e.GetComponent<Transform>();
            t.position = glm::vec3(t.position.z, t.position.y + 0.001f, t.position.z);
        }
    }
};
class RenderSystem : public System {
  public:
    RenderSystem() {
        m_Signature = Simplex::GetRegistry().CreateSignature<Sprite, Transform>();
    }

    void Update() override {
        for (Entity e : m_Entities) {
            Sprite s = e.GetComponent<Sprite>();
            Simplex::GetRenderer().Queue(s);
        }
    }
};

void Simplex::Tick() {

    Entity e = m_Registry.Create();
    Transform t = {.position = glm::vec3(1, 1, 1)};
    Sprite s = {.texture = "GRASS_TILE_1", .postition = glm::vec3(-.5, -.5, 0), .size = glm::vec2(.5, .5), .color = glm::vec4(0, 0, 0, 1)};

    m_Registry.AddComponent<Transform>(e, t);
    m_Registry.AddComponent<Sprite>(e, s);

    while (!m_View.ShouldQuit()) {
        m_Input.PollEvents();

        m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        // Do scene tick here

        m_Renderer.Render();

        m_View.SwapBuffers();
    }
}
