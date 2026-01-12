#pragma once

#include "core/Simplex.h"
#include "core/Types.h"
#include "gui/Bindable.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"
#include <any>
#include <functional>
#include <iostream>

struct DefaultUIProps
{
    UILayout layout;
    UIStyle style;
    Text text;
    UIEvents events;

    bool operator==(const DefaultUIProps &rhs) const = default;
};

struct UISpecification
{
    DefaultUIProps properties;
    std::vector<std::function<void(EntityId)>> extraComponents;

    Bindable<std::vector<UISpecification>> children;

    bool operator==(const UISpecification &rhs) const
    {
        return false;
    };
};

class ElementHandle
{
  public:
    static std::vector<UISpecification *> context;

    template <typename... TComponents>
    ElementHandle(DefaultUIProps props, TComponents &&...extra)
    {
        std::vector<std::function<void(EntityId)>> components;
        (components.push_back([extra](EntityId entity) { Simplex::GetRegistry().QueueComponent(entity, extra); }), ...);

        spec = UISpecification{
            .properties = props,
            .extraComponents = components};

        if(!context.empty())
        {
            auto parent = context.back();
            parent->children.Set([&](std::vector<UISpecification> &children) { children.push_back(spec); });
        }
    }

    ElementHandle &Children(std::function<void()> childrenFn)
    {
        // make this parent
        std::cout << "MAKING THIS PARENT: " << spec.properties.text.content.Get() << "\n";
        context.push_back(&spec);
        childrenFn();
        context.pop_back();
        std::cout << "POPPING THIS PARENT: " << spec.properties.text.content.Get() << "\n";
        return *this;
    }

    ElementHandle &BindChildren(std::function<void()> childBuildFn)
    {
        auto childFn = [=]() {
            // make this parent
            UISpecification tempSpec = {};
            context.push_back(&tempSpec);
            childBuildFn();
            context.pop_back();
            return tempSpec.children.Get();
        };
        spec.children.Set(childFn);

        return *this;
    }

    // Expose spec when needed (root)
    UISpecification Take()
    {
        return spec;
    }

  private:
    UISpecification spec;
};
inline std::vector<UISpecification *> ElementHandle::context;
