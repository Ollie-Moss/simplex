#pragma once

#include "core/Simplex.h"
#include "gui/Text.h"
#include "gui/UIBuilder.h"
#include "core/SystemManager.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "gui/UIComponents.h"
#include <cctype>
#include <cmath>
#include <sys/types.h>
#include <tuple>
#include <vector>
#include "gui/UIComponents.h"

class UIStateSystem : public System
{
  public:
    UIStateSystem()
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, UILayout, UIStyle, Text>();
    }
    void Update(float timeStep) override
    {
        for(Entity e : m_Entities)
        {
            auto [elem, trans, layout, style, text] = e.GetComponents<UIElement, UITransform, UILayout, UIStyle, Text>();

            bool changed = elem.childrenSpec.UpdateBinding();
            if(changed)
            {
                for(auto child : elem.children)
                {
                    DeleteTree(child);
                }
                elem.children.clear();

                // Create new children
                for(auto childSpec : elem.childrenSpec.Get())
                {
                    auto child = BuildUI(Simplex::GetRegistry(), childSpec);
                    elem.children.push_back(child);
                }
            }

            bool anyChanged = false;
            std::apply(
                [&](auto &...b) {
                    ((anyChanged |= b.UpdateBinding()), ...);
                },
                layout.bindables());

            std::apply(
                [&](auto &...b) {
                    ((anyChanged |= b.UpdateBinding()), ...);
                },
                style.bindables());

            std::apply(
                [&](auto &...b) {
                    ((anyChanged |= b.UpdateBinding()), ...);
                },
                text.bindables());

            elem.dirty = (anyChanged || changed);
        }
    }

    void DeleteTree(Entity e)
    {
        auto &elem = e.GetComponent<UIElement>();
        for(auto child : elem.children)
        {
            DeleteTree(child);
        }
        elem.children.clear();
        Simplex::GetRegistry().Destroy(e);
    }
};
