#pragma once

#include "assets/AssetManager.h"
#include "core/IView.h"
#include "core/Scene.h"
#include "Registry.h"
#include "core/View.h"
#include "core/Input.h"
#include "graphics/RendererManager.h"
#include <cassert>

struct SimplexModules
{
    std::unique_ptr<IInput> m_Input = std::make_unique<Input>();
    std::unique_ptr<IView> m_View = std::make_unique<View>();
    std::unique_ptr<AssetManager> m_AssetManager = std::make_unique<AssetManager>();
    std::unique_ptr<RendererManager> m_RendererManager = std::make_unique<RendererManager>();
};

class Simplex
{
  public:
    Simplex(SimplexModules modules = {});
    ~Simplex();

    bool Init();
    void Start();
    void SetScene(const Scene &scene);

    static Simplex &Get();

    static IView &GetView();
    static IInput &GetInput();
    static Scene &GetScene();
    static Registry &GetRegistry();
    static RendererManager &GetRendererManager();
    static AssetManager &GetAssetManager();

    float GetFPS();
    float GetDeltaTime();

  private:
    void Tick();

  private:
    SimplexModules m_Modules;

    Scene m_CurrentScene;

    float m_DeltaTime;
    float m_Fps;

    inline static Simplex *s_Instance = nullptr;
};
