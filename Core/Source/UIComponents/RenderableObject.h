#pragma once

#include "Enums.h"
#include "raylib.h"
#include "DataTypes/Vector2Ex.h"
#include "DataTypes/Bounds.h"

namespace UIComponents
{

    class RenderableObject
    {

    private:
        Vector2Ex<int> m_drawPoint;
        Vector2Ex<int> m_anchorPointPosition;
        AnchorPoint m_anchorPoint;
        bool m_visible;

    protected:
        void UpdateDrawPoint();

    public:
        RenderableObject(Vector2Ex<int> anchorPointPosition, AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT, bool visible = true);
        virtual ~RenderableObject() = default;

        virtual void SetAnchorPointPosition(Vector2Ex<int> position); // Sets anchor point position
        // virtual Vector2 GetPosition() const; // Returns top left position
        virtual Vector2Ex<int> GetPosition(AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT) const;

        virtual void SetAnchorPoint(AnchorPoint anchorPoint);
        virtual AnchorPoint GetAnchorPoint() const;

        virtual void SetVisibility(bool visible);
        virtual bool IsVisible() const;

        virtual void Move(Direction direction, float step = 1);

        virtual Bounds GetHitbox() const;

        virtual Vector2Ex<int> GetDimensions() const = 0;
        virtual void Render(Vector2Ex<int> offset) const = 0;
    };

}