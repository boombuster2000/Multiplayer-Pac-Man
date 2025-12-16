#pragma once

#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

namespace ui
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

  public:
    RenderableObject();
    explicit RenderableObject(const Vector2Ex<float>& anchorPointPosition,
                              AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT,
                              bool visible = true);
    virtual ~RenderableObject() = default;

    /// @return Returns the position of the origin in the object (not in the 2d space).
    [[nodiscard]] virtual Vector2Ex<float> GetObjectOrigin() const;

    /// @return Returns the position of the origin in the 2d space.
    [[nodiscard]] virtual Vector2Ex<float> GetWorldOrigin() const;

    /// @brief Sets the position of the origin in the object (not in the 2d space).
    /// @param anchorPoint Common points in the object from which the position for the origin can be calculated from.
    virtual void SetOrigin(AnchorPoint anchorPoint);

    /// @brief Sets the position of the origin in the object (not in the 2d space).
    /// @param origin A position representing the origin position in the object.
    virtual void SetOrigin(Vector2Ex<float> origin);

    /// @brief Calculates the local origin for the given anchorpoint.
    /// @param anchorPoint Where in the object you want the local origin of.
    /// @param width The width of the object.
    /// @param height The height of the object.
    /// @return Returns the local origin for the given anchorpoint.
    static Vector2Ex<float> CalculateAnchorOffset(AnchorPoint anchorPoint, float width, float height);

    /// @brief Calculates the local origin for the given anchorpoint.
    /// @param anchorPoint Where in the object you want the local origin of.
    /// @param dimensions The pixel dimensions of the object.
    /// @return Returns the local origin for the given anchorpoint.
    static Vector2Ex<float> CalculateAnchorOffset(AnchorPoint anchorPoint, const Vector2Ex<float>& dimensions);

    /// @return Returns the position anchor-points of the object in the 2d space.
    [[nodiscard]] Vector2Ex<float> GetPositionAtAnchor(AnchorPoint anchorPoint = AnchorPoint::TOP_LEFT) const;

    /// @brief Sets the position of the origin in the 2d space.
    /// @param position The position of the origin in the 2d space.
    virtual void SetPosition(Vector2Ex<float> position);

    /// @brief Sets the visibility of the object.
    /// @param visible True for visible, false for invisible.
    virtual void SetVisibility(bool visible);

    /// @return Returns whether the object is visible or not.
    [[nodiscard]] virtual bool IsVisible() const;

    /// @brief Moves the object in a direction by a step amount.
    /// @param direction The direction to move the object in.
    /// @param step The amount to change the position by.
    virtual void Move(Direction direction, float step);

    /// @return Returns the pixel width and pixel height of the object
    [[nodiscard]] virtual Vector2Ex<float> GetDimensions() const = 0;

    /// @brief Draws the object to the screen
    /// @param offset The offset from the world origin to draw at.
    virtual void Render(Vector2Ex<float> offset) const = 0;

    friend void to_json(json&, const RenderableObject&);
    friend void from_json(const json&, RenderableObject&);
};

} // namespace ui