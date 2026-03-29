#pragma once

template <typename T>
struct RectBounds
{
    RectBounds() {}

    RectBounds(T _top, T _right, T _bottom, T _left)
        : top(_top), right(_right), bottom(_bottom), left(_left) {}

    RectBounds(T vertical, T horizontal)
        : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}

    RectBounds(T value)
        : top(value), right(value), bottom(value), left(value) {}

    T top;
    T right;
    T bottom;
    T left;

    bool operator==(const RectBounds<T> &rhs) const
    {
        return (top == rhs.top) &&
               (right == rhs.right) &&
               (bottom == rhs.bottom) &&
               (left == rhs.left);
    }

    bool InBounds(T x, T y)
    {
        return (left <= x && x <= right &&
                bottom <= y && y <= top);
    };

};
