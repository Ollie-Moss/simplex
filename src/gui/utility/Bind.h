#pragma once

#include "gui/utility/Bindable.h"
#include "gui/utility/FrameUpdater.h"
#include <memory>

template <typename T>
Bindable<T> Bind(BindingFunc<T> binding)
{
    return Bindable<T>(binding, std::make_shared<FrameUpdater>());
};
