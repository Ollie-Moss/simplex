#pragma once

#include <functional>
#include <optional>

template <typename T>
using BindingFunc = std::function<T()>;

template <typename T>
class Bindable
{
  public:
    Bindable() = default;

    Bindable(BindingFunc<T> binding)
        : m_Binding(binding) {}

    Bindable(const T &value)
        : m_Value(value) {}

    template <typename U, typename = std::enable_if_t<std::is_assignable_v<T, U>>>
    Bindable(const U &value) : m_Value(value) {}

    bool operator==(const Bindable<T> &rhs) const
    {
        return m_Value == rhs.m_Value;
    }

    // returns true if value changed
    bool UpdateBinding()
    {
        if(!m_Binding.has_value())
            return false;

        T val = m_Binding.value()();
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
    void Set(std::function<void(T &)> fn) { fn(m_Value); }

    void Set(const BindingFunc<T> &binding) { m_Binding = binding; }

  private:
    std::optional<BindingFunc<T>> m_Binding;
    T m_Value;
};

template <typename T>
Bindable<T> Bind(BindingFunc<T> binding)
{
    return Bindable<T>(binding);
};
