#include "UIComponents/GridTile.h"
#include "GridTile.h"
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
    const Vector2Ex<int> &position = GetPosition();
    DrawRectangleV(position + offset, m_dimensions, RED);
}