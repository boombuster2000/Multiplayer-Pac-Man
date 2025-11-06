#include "Bounds.h"

Bounds::Bounds(Vector2Ex<float> dimensions, Vector2Ex<float> position)
    : dimensions(dimensions), position(position)
{
}

bool Bounds::IsPointInBounds(const Vector2Ex<float> &pos) const
{
    return (pos.x >= position.x &&
            pos.x <= position.x + dimensions.x &&
            pos.y >= position.y &&
            pos.y <= position.y + dimensions.y);
}

bool Bounds::IsBoundsTouching(const Bounds &a, const Bounds &b)
{
    const bool xOverlap = a.position.x < b.position.x + b.dimensions.x &&
                          a.position.x + a.dimensions.x > b.position.x;

    const bool yOverlap = a.position.y < b.position.y + b.dimensions.y &&
                          a.position.y + a.dimensions.y > b.position.y;

    return xOverlap && yOverlap;
}
