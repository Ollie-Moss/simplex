#pragma once

#include "core/Renderer2D.h"
#include "Registry.h"
#include "core/ResourceManager.h"
#include "core/View.h"
#include "core/Input.h"
#include <cassert>

class Simplex {
  public:
    Simplex();
    ~Simplex();

    bool Init();
    void Start();

    static Simplex &Get();

    static View &GetView();
    static Input &GetInput();
    static ResourceManager &GetResources();
    static Renderer2D &GetRenderer();
    static Registry &GetRegistry();

  private:
    void Tick();

  private:
    View m_View;
    Input m_Input;
    ResourceManager m_ResourceManager;
    Registry m_Registry;
    Renderer2D m_Renderer;
    inline static Simplex *s_Instance = nullptr;
};
