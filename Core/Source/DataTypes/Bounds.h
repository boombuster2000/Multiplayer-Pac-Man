#pragma once

#include "DataTypes/Vector2Ex.h"

class Bounds
{
public:
    Vector2Ex<float> dimensions;
    Vector2Ex<float> position; // Top left (drawpoint)

public:
    Bounds(Vector2Ex<float> dimensions, Vector2Ex<float> position);
    ~Bounds() = default;

    bool IsPointInBounds(const Vector2Ex<float> &position) const;
    static bool IsBoundsTouching(const Bounds &a, const Bounds &b);
};