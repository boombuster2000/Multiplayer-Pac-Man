#include "RenderableObject.h"

namespace UIComponents
{

    RenderableObject::RenderableObject(Vector2Ex<int> anchorPointPosition, AnchorPoint anchorPoint, bool visible)
        : m_anchorPointPosition(anchorPointPosition), m_visible(visible), m_anchorPoint(anchorPoint)
    {
    }

    void RenderableObject::SetAnchorPointPosition(Vector2Ex<int> position)
    {
        m_anchorPointPosition = position;
        UpdateDrawPoint(); // offsets top left position so that anchor point position is at user set position.
    }

    // Vector2 RenderableObject::GetPosition() const
    //{
    //     return m_position;
    // }

    // Gives top left position by default as a common reference points between objects for calculations
    Vector2Ex<int> RenderableObject::GetPosition(AnchorPoint anchorpoint) const
    {
        using enum AnchorPoint;
        Vector2Ex<int> size = GetDimensions();
        Vector2Ex<int> position = m_drawPoint;

        switch (anchorpoint)
        {
        case TOP_LEFT:
            // No adjustment needed
            break;
        case TOP_MIDDLE:
            position.x += size.x / 2;
            break;
        case TOP_RIGHT:
            position.x += size.x;
            break;
        case MIDDLE_LEFT:
            position.y += size.y / 2;
            break;
        case MIDDLE:
            position.x += size.x / 2;
            position.y += size.y / 2;
            break;
        case MIDDLE_RIGHT:
            position.x += size.x;
            position.y += size.y / 2;
            break;
        case BOTTOM_LEFT:
            position.y += size.y;
            break;
        case BOTTOM_MIDDLE:
            position.x += size.x / 2;
            position.y += size.y;
            break;
        case BOTTOM_RIGHT:
            position.x += size.x;
            position.y += size.y;
            break;
        default:
            break;
        }

        return position;
    }

    void RenderableObject::SetAnchorPoint(AnchorPoint anchorPoint)
    {
        m_anchorPoint = anchorPoint;
        UpdateDrawPoint();
    }

    AnchorPoint RenderableObject::GetAnchorPoint() const
    {
        return m_anchorPoint;
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
            m_anchorPointPosition.y -= step;
            break;

        case DOWN:
            m_anchorPointPosition.y += step;
            break;

        case LEFT:
            m_anchorPointPosition.x -= step;
            break;

        case RIGHT:
            m_anchorPointPosition.x += step;
            break;

        default:
            break;
        }

        UpdateDrawPoint();
    }

    Bounds RenderableObject::GetHitbox() const
    {
        return Bounds(GetDimensions(), GetPosition());
    }

    // Calculates corrected top left position based on the anchor point so that the anchor point of object is at the position set by the user.
    void RenderableObject::UpdateDrawPoint()
    {
        using enum AnchorPoint;
        Vector2Ex<int> size = GetDimensions();

        m_drawPoint = m_anchorPointPosition;

        switch (m_anchorPoint)
        {
        case TOP_LEFT:
            // No changes needed
            break;
        case TOP_MIDDLE:
            m_drawPoint.x -= size.x / 2;
            break;
        case TOP_RIGHT:
            m_drawPoint.x -= size.x;
            break;
        case MIDDLE_LEFT:
            m_drawPoint.y -= size.y / 2;
            break;
        case MIDDLE:
            m_drawPoint.x -= size.x / 2;
            m_drawPoint.y -= size.y / 2;
            break;
        case MIDDLE_RIGHT:
            m_drawPoint.x -= size.x;
            m_drawPoint.y -= size.y / 2;
            break;
        case BOTTOM_LEFT:
            m_drawPoint.y -= size.y;
            break;
        case BOTTOM_MIDDLE:
            m_drawPoint.x -= size.x / 2;
            m_drawPoint.y -= size.y;
            break;
        case BOTTOM_RIGHT:
            m_drawPoint.x -= size.x;
            m_drawPoint.y -= size.y;
            break;
        default:
            break;
        }
    }

}