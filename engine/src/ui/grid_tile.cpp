#include "engine/ui/grid_tile.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"


ui::GridTile::GridTile() : RenderableObject(), m_dimensions(Vector2Ex<float>(0, 0))
{
}

ui::GridTile::GridTile(Vector2Ex<float> dimensions, Vector2Ex<float> position)
    : RenderableObject(position), m_dimensions(dimensions)
{
}

Vector2Ex<float> ui::GridTile::GetDimensions() const
{
    return m_dimensions;
}

void ui::GridTile::Render(Vector2Ex<float> offset) const
{
    const Vector2Ex<float> position = GetWorldOrigin() + offset;
    const Vector2Ex<float> dimensions = GetDimensions();
    const Vector2Ex<float> objectOrigin = GetObjectOrigin();
    const Rectangle rec = {position.x, position.y, dimensions.x, dimensions.y};

    DrawRectanglePro(rec, objectOrigin, 0.0f, RED);
}