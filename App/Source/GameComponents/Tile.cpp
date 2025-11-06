#include "Tile.h"
#include "Core/Application.h"

Tile::Tile(Type type, Vector2Ex<float> position, Vector2Ex<float> dimensions)
    : GridTile(dimensions, position), m_type(type)
{
}

Tile::Type Tile::GetType() const
{
    return m_type;
}

void Tile::SetType(Type type)
{
    m_type = type;
}

void Tile::Render(Vector2Ex<float> offset) const
{
    Vector2Ex<float> position = GetPositionAtAnchor();
    Vector2Ex<float> size = GetDimensions();
    std::shared_ptr<Texture2D> pathTexture = Core::Application::GetTexturesManager()->GetTexture("path");
    std::shared_ptr<Texture2D> wallTexture = Core::Application::GetTexturesManager()->GetTexture("wall");

    if (m_type == Type::Wall)
    {
        DrawTextureEx(*wallTexture, position, 0, size.y / (float)wallTexture->height, WHITE);
    }
    else if (m_type == Type::Path)
    {
        DrawTextureEx(*pathTexture, position, 0, size.y / (float)pathTexture->height, WHITE);
    }
}
