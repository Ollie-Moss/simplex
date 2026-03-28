#pragma once

// UINode.h
//
// Replaces UISpecification + UIBuilderTypes.h
//
// A UINode is a plain, immutable-ish value type that describes a single UI
// element and its children. There is NO global context stack, NO destructor
// side-effects, and NO Bindable<vector<UISpecification>> on the node itself.
//
// Static children  → std::vector<UINode>  (known at declaration time)
// Dynamic children → ChildrenFn           (re-evaluated each frame when dirty)
//
// A ChildrenFn returns a fresh std::vector<UINode>. The reconciler (UITree)
// diffs the returned list against the live entity tree using the node's `key`
// field and only creates / destroys / reorders what actually changed.

#include "gui/UIComponents.h"
#include "gui/UILayoutTypes.h"
#include "gui/Text.h"
#include "gui/Bindable.h"
#include "core/Types.h"

#include <functional>
#include <string>
#include <variant>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// UINodeProps  (was DefaultUIProps)
// ─────────────────────────────────────────────────────────────────────────────

struct UINodeProps
{
    // Stable identity used by the reconciler.
    // If empty, the reconciler falls back to positional identity (like React
    // without keys). Provide explicit keys whenever children can be reordered
    // or inserted in the middle.
    std::string key;

    UILayout    layout;
    UIStyle     style;
    Text        text;
    UIEvents    events;

    bool operator==(const UINodeProps &) const = default;
};

// ─────────────────────────────────────────────────────────────────────────────
// Forward declaration so ChildrenFn can reference UINode
// ─────────────────────────────────────────────────────────────────────────────

struct UINode;

// A function that, when called, produces the current child list.
// Called by UITree::Reconcile every frame for nodes that have dynamic children.
using ChildrenFn = std::function<std::vector<UINode>()>;

// ─────────────────────────────────────────────────────────────────────────────
// UINode
// ─────────────────────────────────────────────────────────────────────────────

struct UINode
{
    UINodeProps props;

    // Extra ECS components to attach (same as before, but a cleaner name).
    // Each entry is a function that receives the EntityId and attaches a component.
    std::vector<std::function<void(EntityId)>> extraComponents;

    // Children: either a static list or a dynamic factory.
    //
    //  • std::vector<UINode>  → evaluated once at construction time; the
    //                           reconciler treats them as stable unless the
    //                           parent itself is recreated.
    //
    //  • ChildrenFn           → called every reconcile pass; return value is
    //                           diffed against the live entity children.
    //                           Use this for data-driven / conditional lists.
    //
    // Only one variant is active at a time. Use the builder helpers below.
    std::variant<std::vector<UINode>, ChildrenFn> children = std::vector<UINode>{};

    // ── Fluent child setters ─────────────────────────────────────────────────

    // Static children: call a lambda that pushes UINodes into a local list.
    //
    //   ui::Box({...})
    //       .Children([]{
    //           return std::vector<UINode>{ ui::Text("hello"), ui::Text("world") };
    //       });
    //
    // Or use the helper overload that accepts an initializer:
    UINode &&WithChildren(std::vector<UINode> staticChildren) &&
    {
        children = std::move(staticChildren);
        return std::move(*this);
    }
    UINode &WithChildren(std::vector<UINode> staticChildren) &
    {
        children = std::move(staticChildren);
        return *this;
    }

    // Dynamic children: supply a ChildrenFn.
    //
    //   ui::Box({...})
    //       .WithDynamicChildren([&myList]{
    //           std::vector<UINode> nodes;
    //           for(auto &item : myList)
    //               nodes.push_back(ui::Text(item.label));
    //           return nodes;
    //       });
    UINode &&WithDynamicChildren(ChildrenFn fn) &&
    {
        children = std::move(fn);
        return std::move(*this);
    }
    UINode &WithDynamicChildren(ChildrenFn fn) &
    {
        children = std::move(fn);
        return *this;
    }

    // ── Accessors used by UITree ─────────────────────────────────────────────

    bool HasDynamicChildren() const
    {
        return std::holds_alternative<ChildrenFn>(children);
    }

    const ChildrenFn &GetChildrenFn() const
    {
        return std::get<ChildrenFn>(children);
    }

    const std::vector<UINode> &GetStaticChildren() const
    {
        return std::get<std::vector<UINode>>(children);
    }
};
