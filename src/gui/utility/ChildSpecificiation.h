#pragma once

#include "gui/utility/UISpecification.h"
#include <initializer_list>
#include <vector>

struct ChildSpecification
{
  public:
    ChildSpecification() {}
    ChildSpecification(const ChildSpecification &other) : m_Elements(std::move(other.m_Elements)) {}

    ChildSpecification(ChildSpecification &&) = default;

    ChildSpecification &operator=(const ChildSpecification &other)
    {
        m_Elements = std::move(other.m_Elements);
        return *this;
    }
    ChildSpecification &operator=(ChildSpecification &&) = default;

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

    const std::vector<UISpecification> &Consolidate() const
    {
        return m_Elements;
    }

  private:
    std::vector<UISpecification> m_Elements;
};
