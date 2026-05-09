#pragma once

#include "core/Simplex.h"
#include "gui/Text.h"
#include "core/Types.h"
#include "gui/UIBuilderTypes.h"
#include "gui/UIComponents.h"
#include <cctype>
#include <cmath>
#include <sys/types.h>
#include <tuple>
#include "gui/UIComponents.h"

class UIStateSystem : public System
{
  public:
    UIStateSystem(Registry &registry, const SimplexModules &modules) : System(registry, modules)
    {
        m_Signature = Simplex::GetRegistry().CreateSignature<UIElement, UITransform, UILayout, UIStyle, Text>();
    }
    void Update(float timeStep) override
    {
        for(EntityId entity : m_Entities)
        {
            // auto [elem, trans, layout, style, text] = e.GetComponents<UIElement, UITransform, UILayout, UIStyle, Text>();
            auto &elem = m_Registry.GetComponent<UIElement &>(entity);
            auto &trans = m_Registry.GetComponent<UITransform &>(entity);
            auto &layout = m_Registry.GetComponent<UILayout &>(entity);
            auto &style = m_Registry.GetComponent<UIStyle &>(entity);
            auto &text = m_Registry.GetComponent<Text &>(entity);

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
                    // auto child = BuildUI(Simplex::GetRegistry(), childSpec);
                    // elem.children.push_back(child);
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

    void DeleteTree(EntityId entity)
    {
        auto &element = m_Registry.GetComponent<UIElement &>(entity);
        for(auto child : element.children)
        {
            DeleteTree(child);
        }
        element.children.clear();
        m_Registry.Destroy(entity);
    }
};
