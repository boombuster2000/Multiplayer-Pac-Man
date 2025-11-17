#include "RenderableObject.h"

namespace UIComponents
{
RenderableObject::RenderableObject()
    : m_worldOrigin(Vector2Ex<float>(0, 0)), m_objectOrigin(Vector2Ex<float>(0, 0)), m_visible(true)
{
}

RenderableObject::RenderableObject(Vector2Ex<float> anchorPointPosition, AnchorPoint anchorPoint, bool visible)
    : m_worldOrigin(anchorPointPosition), m_objectOrigin(Vector2Ex<float>(0, 0)), m_visible(visible)
{
}

Vector2Ex<float> RenderableObject::GetObjectOrigin() const
{
    return m_objectOrigin;
}

Vector2Ex<float> RenderableObject::GetWorldOrigin() const
{
    return m_worldOrigin;
}

void RenderableObject::SetOrigin(AnchorPoint anchorPoint)
{
    using enum AnchorPoint;
    const Vector2Ex<float> &dimensions = GetDimensions();

    m_objectOrigin = CalculateAnchorOffset(anchorPoint, dimensions);
}

void RenderableObject::SetOrigin(Vector2Ex<float> origin)
{
    m_objectOrigin = origin;
}

Vector2Ex<float> RenderableObject::CalculateAnchorOffset(const AnchorPoint anchorPoint, float width, float height)
{
    Vector2Ex<float> offset(0, 0);
    width -= 1;
    height -= 1;

    using enum AnchorPoint;
    switch (anchorPoint)
    {
    case TOP_LEFT:
        break;
    case TOP_MIDDLE:
        offset.x = width / 2;
        break;
    case TOP_RIGHT:
        offset.x = width;
        break;
    case MIDDLE_LEFT:
        offset.y = height / 2;
        break;
    case MIDDLE:
        offset.x = width / 2;
        offset.y = height / 2;
        break;
    case MIDDLE_RIGHT:
        offset.x = width;
        offset.y = height / 2;
        break;
    case BOTTOM_LEFT:
        offset.y = height;
        break;
    case BOTTOM_MIDDLE:
        offset.x = width / 2;
        offset.y = height;
        break;
    case BOTTOM_RIGHT:
        offset.x = width;
        offset.y = height;
        break;
    }

    return offset;
}

Vector2Ex<float> RenderableObject::CalculateAnchorOffset(const AnchorPoint anchorPoint,
                                                         const Vector2Ex<float> &dimensions)
{
    return CalculateAnchorOffset(anchorPoint, dimensions.x, dimensions.y);
}

// Gives top left position by default as a common reference points between objects for calculations
Vector2Ex<float> RenderableObject::GetPositionAtAnchor(AnchorPoint anchorpoint) const
{
    const Vector2Ex<float> &dimensions = GetDimensions();
    Vector2Ex<float> position = m_worldOrigin - m_objectOrigin; // Gets top left position.
    position += CalculateAnchorOffset(anchorpoint, dimensions);

    return position;
}

void RenderableObject::SetPosition(Vector2Ex<float> position)
{
    m_worldOrigin = position;
}

void RenderableObject::SetVisibility(bool visible)
{
    m_visible = visible;
}

bool RenderableObject::IsVisible() const
{
    return m_visible;
}

void RenderableObject::Move(Direction direction, float step)
{
    using enum Direction;

    switch (direction)
    {
    case UP:
        m_worldOrigin.y -= step;
        break;

    case DOWN:
        m_worldOrigin.y += step;
        break;

    case LEFT:
        m_worldOrigin.x -= step;
        break;

    case RIGHT:
        m_worldOrigin.x += step;
        break;

    default:
        break;
    }
}

} // namespace UIComponents