#pragma once

#include "core/Entity.h"
#include <functional>
#include <optional>

template <typename T>
using BindingFunc = std::function<T(std::optional<Entity>)>;

template <typename T>
class Bindable
{
  public:
    Bindable() {}

    Bindable(std::optional<Entity> target, BindingFunc<T> binding)
        : m_Target(target),
          m_Binding(binding)
    {}

    Bindable(const T &value)
        : m_Value(value)
    {}

    template <typename U, typename = std::enable_if<std::is_assignable<U, T>::value>>
    Bindable(const U &value) : m_Value(value)
    {}

    bool operator==(const Bindable<T> &rhs) const
    {
        return m_Value == rhs.m_Value;
    }

    // returns true based on whether the binding provided a different value
    bool UpdateBinding()
    {
        if(!m_Binding.has_value())
            return false;

        auto val = m_Binding.value()(m_Target);
        if(m_Value != val)
        {
            m_Value = val;
            return true;
        }
        return false;
    }

    T &Get() { return m_Value; }
    const T &Get() const { return m_Value; }

    T &operator*() { return m_Value; }
    const T &operator*() const { return m_Value; }

    void Set(const T &value) { m_Value = value; }

    void Set(const BindingFunc<T> &binding) { m_Binding = binding; }

  private:
    std::optional<Entity> m_Target;
    std::optional<BindingFunc<T>> m_Binding;
    T m_Value;
};

template <typename T>
Bindable<T> Bind(std::optional<Entity> entity, BindingFunc<T> binding)
{
    return Bindable<T>(entity, binding);
};

template <typename T>
Bindable<T> Bind(BindingFunc<T> binding)
{
    return Bindable<T>(std::nullopt, binding);
};
