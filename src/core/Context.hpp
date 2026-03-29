#pragma once

#include "Registry.h"
#include "assets/AssetManager.h"
#include "core/View.h"
#include "core/Input.h"
#include "graphics/RendererManager.h"

struct CoreContext
{
    Registry &registry;
    ITime &time;
};

struct RenderContext
{
    RendererManager &renderer;
};

struct InputContext
{
    IInput &input;
};
