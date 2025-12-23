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

    template <typename Vec3, typename Vec2>
    bool IsQuadVisible(const Vec3 &position, const Vec2 &size) const
    {
        // Ignore the z component of the position (only care about x and y)
        T quadLeft = position.x;
        T quadRight = position.x + size.x;
        T quadBottom = position.y;
        T quadTop = position.y + size.y;

        // Check if the quad (the rectangle defined by position and size) is within the RectBounds
        if(quadRight < left || quadLeft > right || quadTop < bottom || quadBottom > top)
            return false;

        // If none of the above, the quad is visible in the RectBounds
        return true;
    }
};
