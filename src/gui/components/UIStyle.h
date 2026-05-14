#pragma once

#include "core/Types.h"
#include "gui/utility/Bindable.h"

template <template <typename> typename Wrapper = std::type_identity_t>
struct UIStyleBase
{
    Wrapper<Color> color = TRANSPARENT;

    bool operator==(const UIStyleBase &rhs) const = default;
};

// This describes the style of a given element that does not effect its final UITransform
using UIStyle = UIStyleBase<>;
using UIStyleDefinition = UIStyleBase<Bindable>;
