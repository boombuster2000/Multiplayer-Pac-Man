#include "UIComponents/GridTile.h"
#include "DataTypes/Vector2Ex.h"

UIComponents::GridTile::GridTile()
    : RenderableObject(), m_dimensions(Vector2Ex<float>(0, 0))
{
}

UIComponents::GridTile::GridTile(Vector2Ex<float> dimensions, Vector2Ex<float> position)
    : RenderableObject(position), m_dimensions(dimensions)
{
}

Vector2Ex<float> UIComponents::GridTile::GetDimensions() const
{
    return m_dimensions;
}

void UIComponents::GridTile::SetDimensions(const Vector2Ex<float>& dimensions)
{
    m_dimensions = dimensions;
}

void UIComponents::GridTile::Render(Vector2Ex<float> offset) const
{
    const Vector2Ex<float> position = GetWorldOrigin() + offset;
    const Vector2Ex<float> dimensions = GetDimensions();
    const Vector2Ex<float> objectOrigin = GetObjectOrigin();
    const Rectangle rec = {position.x, position.y, dimensions.x, dimensions.y};

    DrawRectanglePro(
        rec,
        objectOrigin,
        0.0f,
        RED);
}