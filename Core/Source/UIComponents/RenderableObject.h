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
        /// @brief The position of the origin in the 2d space.
        Vector2Ex<float> m_worldOrigin;

        /// @brief The position inside the object where it's drawn from and rotated around.
        Vector2Ex<float> m_objectOrigin;

        /// @brief Whether the object is visible or not.
        bool m_visible;

    protected:
        // void UpdateDrawPoint();

    public:
        RenderableObject(Vector2Ex<float> anchorPointPosition, AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT, bool visible = true);
        virtual ~RenderableObject() = default;

        /// @return Returns the position of the orgin in the object (not in the 2d space).
        virtual Vector2Ex<float> GetObjectOrigin() const;

        /// @return Returns the position of the origin in the 2d space.
        virtual Vector2Ex<float> GetWorldOrigin() const;

        /// @brief Sets the position of the origin in the object (not in the 2d space).
        /// @param anchorPoint Common points in the object from which the position for the origin can be calculated from.
        virtual void SetOrigin(AnchorPoint anchorPoint);

        /// @brief Sets the position of the origin in the object (not in the 2d space).
        /// @param origin A position representing the origin position in the object.
        virtual void SetOrigin(Vector2Ex<float> origin);

        /// @return Returns the position anchorpoints of the object in the 2d space.
        virtual Vector2Ex<float> GetPositionAtAnchor(AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT) const;

        /// @brief Sets the position of the origin in the 2d space.
        /// @param position The position of the origin in the 2d space.
        virtual void SetPosition(Vector2Ex<float> position);

        /// @brief Sets the visibility of the object.
        /// @param visible True for visible, false for invisible.
        virtual void SetVisibility(bool visible);

        /// @return Returns whether the object is visible or not.
        virtual bool IsVisible() const;

        /// @brief Moves the object in a direction by a step amount.
        /// @param direction The direction to move the object in.
        /// @param step The amount to change the position by.
        virtual void Move(Direction direction, float step = 1);

        /// @brief
        /// @return
        virtual Bounds GetHitbox() const;

        virtual Vector2Ex<float> GetDimensions() const = 0;
        virtual void Render(Vector2Ex<float> offset) const = 0;
    };

}