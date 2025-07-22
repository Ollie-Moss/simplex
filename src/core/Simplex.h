#pragma once

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

  private:
    void Tick();

  private:
    View m_View;
    Input m_Input;
    ResourceManager m_ResourceManager;
    inline static Simplex *s_Instance = nullptr;
};
