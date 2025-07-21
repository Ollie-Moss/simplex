#include "Simplex.h"

Simplex::Simplex() {
    s_Instance = this;
}

bool Simplex::Init() {
    if (!m_View.Init("Window", 1280, 720))
        return false;

    if (!m_Input.Init())
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

void Simplex::Start() {
    Tick();
}

void Simplex::Tick() {
	while (!m_View.ShouldQuit()) {
		m_Input.PollEvents();

		m_View.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        // Do scene tick here

		m_View.SwapBuffers();
	}
}
