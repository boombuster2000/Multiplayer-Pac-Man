#pragma once
#include <concepts>
#include <cmath>
#include <type_traits>
#include "raylib.h"
#include "UIComponents/Enums.h"

template <typename T>
concept NumberLike = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { std::sqrt(a) } -> std::convertible_to<T>;
    { a == b } -> std::convertible_to<bool>;
    { a++ } -> std::convertible_to<T>;
    { ++a } -> std::convertible_to<T>;
    { b-- } -> std::convertible_to<T>;
    { --b } -> std::convertible_to<T>;
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

    constexpr Vector2Ex operator/(const int &o) const
    {
        return {x / o, y / o};
    }

    // get Vector offsets funcs

    Vector2Ex<T> GetShiftedVector(const UIComponents::Direction &directionOffset, const Vector2Ex<T> &offset) const
    {
        using enum UIComponents::Direction;
        const T &xOffset = offset.x;
        const T &yOffset = offset.y;

        switch (directionOffset)
        {
        case UP:
            return Vector2Ex<T>(x, y - yOffset);
        case DOWN:
            return Vector2Ex<T>(x, y + yOffset);
        case LEFT:
            return Vector2Ex<T>(x - xOffset, y);
        case RIGHT:
            return Vector2Ex<T>(x + xOffset, y);
        default:
            return *this;
        }
    }
};
