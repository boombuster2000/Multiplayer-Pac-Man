#include "Bounds.h"

Bounds::Bounds(Vector2Ex<int> dimensions, Vector2Ex<int> position)
    : dimensions(dimensions), position(position)
{
}

bool Bounds::IsPointInBounds(const Vector2Ex<int> &pos) const
{
    if (pos.x > this->position.x && pos.x < this->position.x + this->dimensions.x)
        if (pos.y < this->position.y && pos.y > this->position.y + this->dimensions.y)
            return true;

    return false;
}

bool Bounds::IsBoundsTouching(const Bounds &a, const Bounds &b)
{

    // TopLeft
    if (b.IsPointInBounds(a.position))
        return true;

    // TopRight
    if (b.IsPointInBounds(Vector2Ex<int>(a.position.x + a.dimensions.x, a.position.y)))
        return true;

    // BottomLeft
    if (b.IsPointInBounds(Vector2Ex<int>(a.position.x, a.position.y + a.dimensions.y)))
        return true;

    // BottomRight
    if (b.IsPointInBounds(Vector2Ex<int>(a.position.x + a.dimensions.x, a.position.y + a.dimensions.y)))
        return true;

    return false;
}
