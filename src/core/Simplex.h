#pragma once

#include "assets/AssetManager.h"
#include "core/Scene.h"
#include "Registry.h"
#include "core/ResourceManager.h"
#include "core/View.h"
#include "core/Input.h"
#include "graphics/RendererManager.h"
#include <cassert>

class Simplex
{
  public:
    Simplex();
    ~Simplex();

    bool Init();
    void Start();
    void SetScene(const Scene &scene);

    static Simplex &Get();

    static View &GetView();
    static Input &GetInput();
    static Scene &GetScene();
    static Registry &GetRegistry();
    static RendererManager &GetRendererManager();
    static AssetManager &GetAssetManager();

    float GetFPS();
    float GetDeltaTime();

  private:
    void Tick();

  private:
    View m_View;
    Input m_Input;
    AssetManager m_AssetManager;
    RendererManager m_RendererManager;

    Scene m_CurrentScene;

    float m_DeltaTime;
    float m_Fps;

    inline static Simplex *s_Instance = nullptr;
};
