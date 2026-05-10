#pragma once

#include "gui/utility/IUpdater.h"
#include "gui/utility/NeverUpdater.h"
#include <functional>
#include <memory>

template <typename T>
using BindingFunc = std::function<T()>;

template <typename T>
class Bindable
{
  public:
    Bindable() = default;

    Bindable(const Bindable &) = default;
    Bindable(Bindable &&) = default;

    Bindable &operator=(const Bindable &) = default;
    Bindable &operator=(Bindable &&) = default;

    // --------------
    // - Value
    // --------------
    Bindable(const T &value)
        : m_Value(value),
          m_Updater(std::make_shared<NeverUpdater>()) {}

    template <typename U, typename = std::enable_if_t<std::is_assignable_v<T, U>>>
    Bindable(const U &value)
        : m_Value(value),
          m_Updater(std::make_shared<NeverUpdater>()) {}

    // -- Custom Updater
    Bindable(const T &value, std::shared_ptr<IUpdater> updater)
        : m_Value(value),
          m_Updater(updater) {}

    template <typename U, typename = std::enable_if_t<std::is_assignable_v<T, U>>>
    Bindable(const U &value, std::shared_ptr<IUpdater> updater)
        : m_Value(value),
          m_Updater(updater) {}

    // --------------
    // - Binding Func
    // --------------
    Bindable(const BindingFunc<T> &binding)
        : m_Binding(std::make_shared<BindingFunc<T>>(binding)),
          m_Value(),
          m_Updater(std::make_shared<NeverUpdater>()) {}

    // -- Custom Updater
    Bindable(const BindingFunc<T> &binding, std::shared_ptr<IUpdater> updater)
        : m_Binding(std::make_shared<BindingFunc<T>>(binding)),
          m_Value(),
          m_Updater(updater) {}

    T &Get()
    {
        UpdateBinding();
        return m_Value;
    }

    void Set(const T &value) { m_Value = value; }
    void Set(const BindingFunc<T> &binding) { m_Binding = std::make_shared<BindingFunc<T>>(binding); }

    // bool operator==(const Bindable<T> &rhs) const { return Get() == rhs.Get(); }
    bool operator==(Bindable<T> &rhs) { return Get() == rhs.Get(); }
    bool operator==(Bindable<T> &&rhs) { return Get() == rhs.Get(); }

  private:
    void UpdateBinding()
    {
        if(m_Binding == nullptr)
            return;

        if(!m_Updater->ShouldUpdate())
            return;

        T updatedValue = (*m_Binding)();
        bool hasChanged = m_Value != updatedValue;

        if(hasChanged)
            m_Value = updatedValue;
    }

    std::shared_ptr<BindingFunc<T>> m_Binding;
    std::shared_ptr<IUpdater> m_Updater;
    T m_Value;
};
