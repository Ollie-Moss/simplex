#pragma once

#include "core/Types.h"
#include "gui/utility/Bind.h"
#include "gui/utility/Bindable.h"
#include "gui/utility/UISpecification.h"
#include <functional>

inline UISpecification TextElement(std::string text, Color color = BLACK)
{
    return UISpecification()
        .Configure(UIElementProperties()
                       .WithText({.content = text, .color = color}));
}

inline UISpecification TextElement(std::function<std::string()> textFn, Color color = BLACK)
{
    return UISpecification()
        .Configure(UIElementProperties()
                       .WithText({.content = Bind<std::string>(textFn), .color = color}));
}
