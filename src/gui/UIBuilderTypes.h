#pragma once

#include "gui/Bindable.h"
#include "gui/Text.h"
#include "gui/UIComponents.h"

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
    std::tuple<> extraComponents;

    Bindable<std::vector<UISpecification>> children;

    void AddChild(const UISpecification &spec)
    {
        children.Get().push_back(spec);
    }

    bool operator==(const UISpecification &rhs) const = default;
};

class ElementHandle
{
  public:
    static std::vector<UISpecification *> context;

    template <typename... TComponents>
    ElementHandle(DefaultUIProps props, TComponents &&...extra)
    {
        spec = UISpecification{.properties = props};
        if(!context.empty())
        {
            auto parent = context.back();
            parent->children.Set([&](std::vector<UISpecification> &children) { children.push_back(spec); });
        }
    }

    ElementHandle &Children(std::function<void()> childrenFn)
    {
        // make this parent
        context.push_back(&spec);
        childrenFn();
        context.pop_back();
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
