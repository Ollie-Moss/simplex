#pragma once

#include "core/Scene.h"
#include "Registry.h"
#include "core/ResourceManager.h"
#include "core/View.h"
#include "core/Input.h"
#include "graphics/Renderer2D.h"
#include <cassert>

class Renderer2D;

class Simplex {
  public:
    Simplex();
    ~Simplex();

    bool Init();
    void Start();
    void SetScene(const Scene &scene);

    static Simplex &Get();

    static View &GetView();
    static Input &GetInput();
    static ResourceManager &GetResources();
    static Renderer2D &GetRenderer();
    static Scene &GetScene();
    static Registry &GetRegistry();

  private:
    void Tick();

  private:
    View m_View;
    Input m_Input;
    ResourceManager m_ResourceManager;
    Renderer2D m_Renderer;

    Scene m_CurrentScene;

    inline static Simplex *s_Instance = nullptr;
};
