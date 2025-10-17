#pragma once

#include "DataTypes/Vector2Ex.h"

class Bounds
{
public:
    Vector2Ex<int> dimensions;
    Vector2Ex<int> position; // Top left (drawpoint)

public:
    Bounds(Vector2Ex<int> dimensions, Vector2Ex<int> position);
    ~Bounds() = default;

    bool IsPointInBounds(const Vector2Ex<int> &position) const;
    static bool IsBoundsTouching(const Bounds &a, const Bounds &b);
};