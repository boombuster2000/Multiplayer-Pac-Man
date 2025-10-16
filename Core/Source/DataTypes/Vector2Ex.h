#pragma once
#include "raylib.h"
#include <concepts>
#include <cmath>
#include <type_traits>

template <typename T>
concept NumberLike = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { std::sqrt(a) } -> std::convertible_to<T>;
    { a == b } -> std::convertible_to<bool>;
};

template <NumberLike T>
class Vector2Ex
{
public:
    T x;
    T y;

public:
    // Constructors/Deconstructors
    constexpr Vector2Ex(T x = 0, T y = 0)
        : x(x), y(y)
    {
    }

    constexpr Vector2Ex(const Vector2 &v)
        : x(static_cast<T>(v.x)), y(static_cast<T>(v.y))
    {
    }

    ~Vector2Ex() = default;

    // Casting to raylib vector2
    operator Vector2() const
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    // Mathmatical Operators
    constexpr Vector2Ex operator+(const Vector2Ex &o) const
    {
        return {x + o.x, y + o.y};
    }

    constexpr Vector2Ex operator-(const Vector2Ex &o) const
    {
        return {x - o.x, y - o.y};
    }

    constexpr Vector2Ex operator*(const Vector2Ex &o) const
    {
        return {x * o.x, y * o.y};
    }

    constexpr Vector2Ex operator/(const Vector2Ex &o) const
    {
        return {x / o.x, y / o.y};
    }
};
