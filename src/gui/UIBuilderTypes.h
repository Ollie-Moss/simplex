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
        (components.push_back([extra](EntityId entity) {
            Simplex::GetRegistry().QueueComponent(entity, extra);
        }),
         ...);

        spec = UISpecification{
            .properties = props,
            .extraComponents = components};
    }

    // Destructor adds to parent AFTER the element is fully built
    ~ElementHandle()
    {
        if(!context.empty() && !taken)
        {
            auto parent = context.back();
            auto children = parent->children.Get();
            children.push_back(spec);
            parent->children.Set(children);
        }
    }

    // Prevent copying to avoid double-add
    ElementHandle(const ElementHandle &) = delete;
    ElementHandle &operator=(const ElementHandle &) = delete;

    // Allow moving
    ElementHandle(ElementHandle &&other) noexcept
        : spec(std::move(other.spec)), taken(other.taken)
    {
        other.taken = true; // Prevent moved-from object from adding to parent
    }

    ElementHandle &operator=(ElementHandle &&other) noexcept
    {
        if(this != &other)
        {
            spec = std::move(other.spec);
            taken = other.taken;
            other.taken = true;
        }
        return *this;
    }

    ElementHandle &Children(std::function<void()> childrenFn) &
    {
        std::cout << "MAKING THIS PARENT: " << spec.properties.text.content.Get() << "\n";
        context.push_back(&spec);
        childrenFn();
        context.pop_back();
        std::cout << "POPPING THIS PARENT: " << spec.properties.text.content.Get() << "\n";
        return *this;
    }
    ElementHandle &&Children(std::function<void()> childrenFn) &&
    {
        std::cout << "MAKING THIS PARENT: " << spec.properties.text.content.Get() << "\n";
        context.push_back(&spec);
        childrenFn();
        context.pop_back();
        std::cout << "POPPING THIS PARENT: " << spec.properties.text.content.Get() << "\n";
        return std::move(*this);
    }

    ElementHandle &BindChildren(std::function<void()> childBuildFn)
    {
        spec.children.Set([childBuildFn]() {
            // Create isolated context for building bound children
            UISpecification tempParent;
            auto savedContext = context;
            context = {&tempParent};

            // Build children - they'll add themselves via destructor
            childBuildFn();

            // Restore context
            context = savedContext;

            return tempParent.children.Get();
        });

        return *this;
    }

    UISpecification Take()
    {
        taken = true; // Prevent destructor from adding to parent
        return spec;
    }

  private:
    UISpecification spec;
    bool taken = false;
};

inline std::vector<UISpecification *> ElementHandle::context;
