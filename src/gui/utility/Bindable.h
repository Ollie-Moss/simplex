#pragma once

#include "core/SimplexModules.h"

template <typename T>
using BindingFunc = T (*)(SimplexModules);

template <typename T>
class Bindable
{
  public:
    Bindable() = default;

    Bindable(const Bindable &) = default;
    Bindable(Bindable &&) = default;

    Bindable &operator=(const Bindable &) = default;
    Bindable &operator=(Bindable &&) = default;

    Bindable(const T &value)
        : value(value) {}

    template <typename U, typename = std::enable_if_t<std::is_assignable_v<T, U>>>
    Bindable(const U &value)
        : value(value) {}

    Bindable(const BindingFunc<T> &binding)
        : binding(binding),
          value() {}

    BindingFunc<T> binding;
    T value;

    bool isDynamic;
};
