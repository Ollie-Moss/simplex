#pragma once

#include "core/Types.h"
#include "gui/utility/ChildSpecificiation.h"
#include "gui/utility/ComponentUpdater.h"
#include "gui/utility/UIElementProperties.h"
#include "gui/utility/UISpecification.h"

inline UISpecification TextElement(std::string text, Color color = BLACK)
{
    return UISpecification()
        .Configure(UIElementProperties()
                       .WithText(TextDefinition().Configure({.content = text, .color = color})));
}

inline UISpecification TextElement(PropertyUpdaterFunc<std::string> textFn, Color color = BLACK)
{
    return UISpecification()
        .Configure(UIElementProperties()
                       .WithText(TextDefinition()
                                     .Configure({.color = color})
                                     .Bind<std::string>(&Text::content, textFn) //
                                 )                                              //
        );
}
