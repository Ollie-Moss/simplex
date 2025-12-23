#include "RendererManager.h"

#include "graphics/render-commands/ColliderCommand.h"
#include "graphics/render-commands/SpriteCommand.h"
#include "graphics/render-commands/TextCommand.h"
#include "graphics/renderers/ColliderRenderer.h"
#include "graphics/renderers/SpriteRenderer.h"
#include "graphics/renderers/TextRenderer.h"

bool RendererManager::Init()
{
    Register<SpriteCommand, SpriteRenderer>();
    Register<TextCommand, TextRenderer>();
    Register<ColliderCommand, ColliderRenderer>();

    return true;
}
