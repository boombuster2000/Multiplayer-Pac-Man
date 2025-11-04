#include "UIComponents/GridTile.h"
#include "DataTypes/Vector2Ex.h"

UIComponents::GridTile::GridTile(Vector2Ex<int> dimensions, Vector2Ex<int> position)
    : RenderableObject(position), m_dimensions(dimensions)
{
}

Vector2Ex<int> UIComponents::GridTile::GetDimensions() const
{
    return m_dimensions;
}

void UIComponents::GridTile::Render(Vector2Ex<int> offset) const
{
    const Vector2Ex<int>& position = GetWorldOrigin();
    const Vector2Ex<int>& dimensions = GetDimensions();
    const Vector2Ex<int>& objectOrigin = GetObjectOrigin();

    DrawRectanglePro(
        { static_cast<float>(position.x + offset.x), static_cast<float>(position.y + offset.y), static_cast<float>(dimensions.x), static_cast<float>(dimensions.y) },
        { static_cast<float>(objectOrigin.x), static_cast<float>(objectOrigin.y) },
        0.0f,
        RED);
}