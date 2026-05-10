#pragma once

#include "gui/utility/UISpecification.h"
#include <initializer_list>
#include <vector>

class ChildSpecification
{
  public:
    ChildSpecification(const UISpecification &element)
    {
        m_Elements.push_back(element);
    }

    ChildSpecification(const std::vector<UISpecification> &elements)
    {
        m_Elements = std::move(elements);
    }

    ChildSpecification(std::initializer_list<UISpecification> elements)
    {
        elements = std::move(elements);
    }

    std::vector<UISpecification> Consolidate() const
    {
        return m_Elements;
    }

  private:
    std::vector<UISpecification> m_Elements;
};
