#include "RenderableObject.h"

namespace UIComponents
{
    RenderableObject::RenderableObject()
    : m_worldOrigin(Vector2Ex<float>(0,0)), m_objectOrigin(Vector2Ex<float>(0, 0)), m_visible(true)
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

        const float width = dimensions.x;
        const float height = dimensions.y;

        const float halfWidth = width / 2;
        const float halfHeight = height / 2;

        switch (anchorPoint)
        {
        case TOP_LEFT:
            m_objectOrigin = Vector2Ex<float>(0, 0);
            break;
        case TOP_MIDDLE:
            m_objectOrigin = Vector2Ex<float>(halfWidth, 0);
            break;
        case TOP_RIGHT:
            m_objectOrigin = Vector2Ex<float>(width, 0);
            break;
        case MIDDLE_LEFT:
            m_objectOrigin = Vector2Ex<float>(0, halfHeight);
            break;
        case MIDDLE:
            m_objectOrigin = Vector2Ex<float>(halfWidth, halfHeight);
            break;
        case MIDDLE_RIGHT:
            m_objectOrigin = Vector2Ex<float>(width, halfHeight);
            break;
        case BOTTOM_LEFT:
            m_objectOrigin = Vector2Ex<float>(0, height);
            break;
        case BOTTOM_MIDDLE:
            m_objectOrigin = Vector2Ex<float>(halfWidth, height);
            break;
        case BOTTOM_RIGHT:
            m_objectOrigin = Vector2Ex<float>(width, height);
            break;
        default:
            break;
        }
    }

    void RenderableObject::SetOrigin(Vector2Ex<float> origin)
    {
        m_objectOrigin = origin;
    }

    // Gives top left position by default as a common reference points between objects for calculations
    Vector2Ex<float> RenderableObject::GetPositionAtAnchor(AnchorPoint anchorpoint) const
    {
        using enum AnchorPoint;
        // Done manually because a dot appears when moving up and left.
        const Vector2Ex<float> &dimensions = GetDimensions(); // - Vector2Ex<float>(1, 1); // This is the pixel before the next tile.;
        Vector2Ex<float> position = m_worldOrigin - m_objectOrigin;

        switch (anchorpoint)
        {
        case TOP_LEFT:
            // No adjustment needed
            break;
        case TOP_MIDDLE:
            position.x += dimensions.x / 2;
            break;
        case TOP_RIGHT:
            position.x += dimensions.x - 1;
            break;
        case MIDDLE_LEFT:
            position.y += dimensions.y / 2;
            break;
        case MIDDLE:
            position.x += dimensions.x / 2;
            position.y += dimensions.y / 2;
            break;
        case MIDDLE_RIGHT:
            position.x += dimensions.x - 1;
            position.y += dimensions.y / 2;
            break;
        case BOTTOM_LEFT:
            position.y += dimensions.y - 1;
            break;
        case BOTTOM_MIDDLE:
            position.x += dimensions.x / 2;
            position.y += dimensions.y - 1;
            break;
        case BOTTOM_RIGHT:
            position.x += dimensions.x - 1;
            position.y += dimensions.y - 1;
            break;
        default:
            break;
        }

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

}