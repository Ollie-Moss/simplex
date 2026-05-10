#pragma once

#include <memory>
#include "assets/AssetManager.h"
#include "core/ITime.h"
#include "core/Time.h"
#include "graphics/RendererManager.h"
#include "core/IInput.h"
#include "core/IView.h"
#include "core/View.h"
#include "core/Input.h"

struct SimplexModules
{
    std::shared_ptr<IInput> m_Input = std::make_shared<Input>();
    std::shared_ptr<IView> m_View = std::make_shared<View>();
    std::shared_ptr<AssetManager> m_AssetManager = std::make_shared<AssetManager>();
    std::shared_ptr<RendererManager> m_RendererManager = std::make_shared<RendererManager>();
    std::shared_ptr<ITime> m_Time = std::make_shared<Time>();

    SimplexModules() = default;
    ~SimplexModules() = default;

    SimplexModules(const SimplexModules &) = default;
    SimplexModules(SimplexModules &&) = default;

    SimplexModules &operator=(const SimplexModules &) = default;
    SimplexModules &operator=(SimplexModules &&) = default;
};
