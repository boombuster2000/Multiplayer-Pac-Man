#pragma once
#include "vector2ex.h"
#include <functional>

namespace std
{

template <typename T>
struct hash<Vector2Ex<T>>
{
    size_t operator()(const Vector2Ex<T>& v) const noexcept
    {
        // Crucial: strong hash combination to reduce collisions
        const size_t hx = std::hash<T>{}(v.x);
        const size_t hy = std::hash<T>{}(v.y);

        // Mix using 64-bit splitmix64-style combining
        size_t h = hx + 0x9e3779b97f4a7c15ULL;
        h ^= hy + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
};

// Optional but explicit, relies on your operator==
template <typename T>
struct equal_to<Vector2Ex<T>>
{
    bool operator()(const Vector2Ex<T>& a, const Vector2Ex<T>& b) const noexcept
    {
        return a == b;
    }
};

} // namespace std
