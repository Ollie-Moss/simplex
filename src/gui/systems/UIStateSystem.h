#pragma once

#include "core/Simplex.h"
#include "gui/components/Text.h"
#include "core/Types.h"
#include <cctype>
#include <cmath>
#include <sys/types.h>
#include "gui/components/UIElement.h"
#include "gui/components/UITransform.h"

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
            UIElement &elem = m_Registry.GetComponent<UIElement>(entity);
            UITransform &trans = m_Registry.GetComponent<UITransform>(entity);
            UILayout &layout = m_Registry.GetComponent<UILayout>(entity);
            UIStyle &style = m_Registry.GetComponent<UIStyle>(entity);
            Text &text = m_Registry.GetComponent<Text>(entity);
        }
    }

    void DeleteTree(EntityId entity)
    {
        UIElement &element = m_Registry.GetComponent<UIElement>(entity);
        for(auto child : element.children)
        {
            DeleteTree(child);
        }
        element.children.clear();
        m_Registry.Destroy(entity);
    }
};
