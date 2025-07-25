template <typename T>
struct RectBounds {
	T top;
	T right;
	T bottom;
	T left;

	bool InBounds(T x, T y) {
		return (left <= x && x <= right &&
				bottom <= y && y <= top);
	};
};
