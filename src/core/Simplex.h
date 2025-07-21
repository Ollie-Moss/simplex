#pragma once

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

  private:
    void Tick();

  private:
    View m_View;
    Input m_Input;
    inline static Simplex *s_Instance = nullptr;
};
