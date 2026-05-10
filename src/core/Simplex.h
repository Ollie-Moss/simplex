#pragma once

#include "assets/AssetManager.h"
#include "core/ITime.h"
#include "core/IView.h"
#include "core/Scene.h"
#include "Registry.h"
#include "core/SimplexModules.h"
#include "graphics/RendererManager.h"
#include <cassert>
#include <optional>

class Simplex
{
  public:
    Simplex(const SimplexModules &modules = {});
    ~Simplex();

    bool Init();
    void Start();
    void SetScene(const Scene &scene);

    static Simplex &Get();

    static IView &GetView();
    static IInput &GetInput();
    static RendererManager &GetRendererManager();
    static AssetManager &GetAssetManager();
    static ITime &GetTime();

    static std::optional<Scene> &GetScene();
    static Registry &GetRegistry();

    static SimplexModules &GetModules();

    float GetFPS();
    float GetDeltaTime();

  private:
    void Tick();

  private:
    SimplexModules m_Modules;

    std::optional<Scene> m_CurrentScene;

    float m_DeltaTime;
    float m_Fps;

    inline static Simplex *s_Instance = nullptr;
};
