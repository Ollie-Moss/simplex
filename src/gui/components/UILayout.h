#pragma once

#include "core/Registry.h"
#include "core/SimplexModules.h"
#include "core/Types.h"
#include "glm/common.hpp"
#include "gui/utility/Bindable.h"
#include "gui/utility/Sizing.h"
#include "gui/utility/Direction.h"
#include "gui/utility/AlignItems.h"
#include "gui/utility/JustifyContent.h"
#include <memory>

// template <template <typename> typename Wrapper = std::type_identity_t>
// struct UILayoutBase
// {
//     Wrapper<Sizing> sizing;
//     Wrapper<Direction> direction = Direction::Horizontal;
//     Wrapper<Padding> padding = Padding(0.0f);
//     Wrapper<float> gap = 0.0f;
//
//     Wrapper<AlignItems> alignItems = AlignItems::Start;
//     Wrapper<JustifyContent> justifyContent = JustifyContent::Start;
// };
struct UILayout
{
    Sizing sizing;
    Direction direction = Direction::Horizontal;
    Padding padding = Padding(0.0f);
    float gap = 0.0f;

    AlignItems alignItems = AlignItems::Start;
    JustifyContent justifyContent = JustifyContent::Start;
};


// // This describes the flex layout properties of a given element
// using UILayout = UILayoutBase<>;
// using UILayoutDefinition = UILayoutBase<Bindable>;
