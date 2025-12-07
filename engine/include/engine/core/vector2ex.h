#pragma once
#include "engine/ui/enums.h"
#include "raylib.h"
#include <cmath>
#include <concepts>
#include <type_traits>

template <typename T>
concept NumberLike = requires(T a, T b) {
    // Unary operators
    { -a } -> std::convertible_to<T>;
    { a++ } -> std::convertible_to<T>;
    { ++a } -> std::convertible_to<T>;
    { a-- } -> std::convertible_to<T>;
    { --a } -> std::convertible_to<T>;

    // Binary operators
    { a + b } -> std::convertible_to<T>;
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
    { a == b } -> std::convertible_to<bool>;

    // Functions
    { std::sqrt(a) } -> std::convertible_to<T>;
};

template <NumberLike T>
class Vector2Ex
{
  public:
    T x;
    T y;

  public:
    // Constructors/Deconstructors
    constexpr Vector2Ex(T x = 0, T y = 0) :
        x(x),
        y(y)
    {
    }

    constexpr Vector2Ex(const Vector2& v) :
        x(static_cast<T>(v.x)),
        y(static_cast<T>(v.y))
    {
    }

    ~Vector2Ex() = default;

    // Casting to raylib vector2
    operator Vector2() const
    {
        return {static_cast<float>(x), static_cast<float>(y)};
    }

    // ---------------------------------------
    // Cross-type binary operators
    // ---------------------------------------
    template <NumberLike U>
    constexpr Vector2Ex operator+(const Vector2Ex<U>& o) const
    {
        return {static_cast<T>(x + o.x), static_cast<T>(y + o.y)};
    }

    template <NumberLike U>
    constexpr Vector2Ex operator-(const Vector2Ex<U>& o) const
    {
        return {static_cast<T>(x - o.x), static_cast<T>(y - o.y)};
    }

    template <NumberLike U>
    constexpr Vector2Ex operator*(const Vector2Ex<U>& o) const
    {
        return {static_cast<T>(x * o.x), static_cast<T>(y * o.y)};
    }

    template <NumberLike U>
    constexpr Vector2Ex operator/(const Vector2Ex<U>& o) const
    {
        return {static_cast<T>(x / o.x), static_cast<T>(y / o.y)};
    }

    // ---------------------------------------
    // Cross-type assignment operators
    // ---------------------------------------
    template <NumberLike U>
    constexpr Vector2Ex& operator+=(const Vector2Ex<U>& o)
    {
        x = static_cast<T>(x + o.x); // why: avoid narrowing and allow U!=T
        y = static_cast<T>(y + o.y);
        return *this;
    }

    template <NumberLike U>
    constexpr Vector2Ex& operator-=(const Vector2Ex<U>& o)
    {
        x = static_cast<T>(x - o.x);
        y = static_cast<T>(y - o.y);
        return *this;
    }

    template <NumberLike U>
    constexpr Vector2Ex& operator*=(const Vector2Ex<U>& o)
    {
        x = static_cast<T>(x * o.x);
        y = static_cast<T>(y * o.y);
        return *this;
    }

    template <NumberLike U>
    constexpr Vector2Ex& operator/=(const Vector2Ex<U>& o)
    {
        x = static_cast<T>(x / o.x);
        y = static_cast<T>(y / o.y);
        return *this;
    }

    // ---------------------------------------
    // Unary minus
    // ---------------------------------------
    constexpr Vector2Ex operator-() const
    {
        return {-x, -y};
    }

    // ---------------------------------------
    // Comparison
    // ---------------------------------------
    constexpr bool operator==(const Vector2Ex& o) const
    {
        return x == o.x && y == o.y;
    }

    constexpr bool operator!=(const Vector2Ex& o) const
    {
        return !(*this == o);
    }

    // ---------------------------------------
    // Scalar ops
    // ---------------------------------------
    template <NumberLike U>
    constexpr Vector2Ex operator*(const U& scalar) const
    {
        return {static_cast<T>(x * scalar), static_cast<T>(y * scalar)};
    }

    template <NumberLike U>
    constexpr Vector2Ex operator/(const U& scalar) const
    {
        return {static_cast<T>(x / scalar), static_cast<T>(y / scalar)};
    }

    // Methods
    static Vector2Ex<T> GetDirectionVector(const ui::Direction& direction)
    {
        using enum ui::Direction;
        using namespace ui;

        switch (direction)
        {
        case UP:
            return {static_cast<T>(0), static_cast<T>(-1)};
        case DOWN:
            return {static_cast<T>(0), static_cast<T>(1)};
        case LEFT:
            return {static_cast<T>(-1), static_cast<T>(0)};
        case RIGHT:
            return {static_cast<T>(1), static_cast<T>(0)};

        default:
            return {static_cast<T>(0), static_cast<T>(0)}; // neutral / no movement
        };
    }

    T GetLength() const
    {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    T GetLengthSqr() const
    {
        return x * x + y * y;
    }

    Vector2Ex<T> GetUnitVector() const
    {
        T len = GetLength();
        if (len == static_cast<T>(0))
            return {0, 0}; // avoid division by zero

        return {x / len, y / len};
    }

    Vector2Ex<T> GetShiftedVector(const ui::Direction& directionOffset, const Vector2Ex<T>& offset) const
    {
        using enum ui::Direction;
        const T& xOffset = offset.x;
        const T& yOffset = offset.y;

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
