template <typename T>
struct RectBounds
{
    T top;
    T right;
    T bottom;
    T left;

    bool InBounds(T x, T y)
    {
        return (left <= x && x <= right &&
                bottom <= y && y <= top);
    };
};

template <>
struct RectBounds<float>
{
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;

    bool InBounds(float x, float y)
    {
        return (left <= x && x <= right &&
                bottom <= y && y <= top);
    };
};
