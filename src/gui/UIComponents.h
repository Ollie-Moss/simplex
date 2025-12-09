#pragma once

#include "components/Transform.h"
#include "core/Entity.h"
#include "core/Types.h"
#include "glm/fwd.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

// clang-format off
enum class AlignItems { Start, End, Center, Stretch };
enum class JustifyContent { Start, End, Center, SpaceBetween, SpaceAround };
enum class Direction { Horizontal, Vertical };
enum class SizingMode { Fixed, Hug, Grow };
enum class Unit { Pixels, Percent };
// clang-format on

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

    template <typename U, typename = std::enable_if<std::is_assignable<T, U>::value>>
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

struct UIElement
{
    EntityId parent = NULL_ENTITY;
    std::vector<EntityId> children;
    bool dirty = true;
};

struct SizeValue
{
    SizeValue() {}
    SizeValue(float value, Unit unit)
        : value(value), unit(unit)
    {}

    float GetValue() const
    {
        return (unit == Unit::Pixels) ? value : value / 100.0f;
    }

    bool operator==(const SizeValue &rhs) const
    {
        return GetValue() == rhs.GetValue();
    }

    float value = 100.0f;
    Unit unit = Unit::Pixels;
};

struct Axis
{
    Axis() {}
    Axis(SizingMode mode, SizeValue length) : mode(mode), length(length) {}

    bool operator==(const Axis &rhs) const
    {
        return (mode == rhs.mode) && (length == rhs.length);
    }

    SizingMode mode = SizingMode::Hug;
    SizeValue length;
};

inline SizeValue Percent(float value) { return SizeValue(value, Unit::Percent); }
inline SizeValue Pixels(float value) { return SizeValue(value, Unit::Percent); }

const Axis GROW = Axis(SizingMode::Grow, Percent(100.0f));
const Axis HUG = Axis(SizingMode::Hug, Pixels(0.0f));

struct Sizing
{
    Bindable<Axis> width;
    Bindable<Axis> height;

    bool operator==(const Sizing &rhs) const
    {
        return (width == rhs.width) && (height == rhs.height);
    }
};

struct Text
{
    std::string fontName = "Arial";
    Bindable<std::string> content = "";
    float fontSize = 12;
    Color color = BLACK;
    float lineHeight = 20.0f;

    std::vector<int> breaks;

    auto bindables()
    {
        return std::tie(content);
    }
};

// This describes the flex layout properties of a given element
struct UILayout
{
    Bindable<Sizing> sizing;
    Bindable<Direction> direction = Direction::Horizontal;
    Bindable<Padding> padding = Padding(0.0f);
    Bindable<float> gap = 0.0f;

    Bindable<AlignItems> alignItems = AlignItems::Start;
    Bindable<JustifyContent> justifyContent = JustifyContent::Start;

    auto bindables()
    {
        return std::tie(
            direction,
            gap,
            justifyContent,
            alignItems,
            padding,
            sizing,
            sizing.Get().width,
            sizing.Get().height);
    }
};

// This describes the style of a given element that does not effect its final UITransform
struct UIStyle
{
    Color color = BLUE;
};

// This component describes the actual rendered dimensions of a given elements
// These values are calculated by the UI layout systems based on the UILayout properties
struct UITransform
{
    glm::vec2 position = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(10, 10);

    operator Transform()
    {
        return Transform{.position = glm::vec3(position.x, position.y, 0),
                         .size = size};
    }
};
