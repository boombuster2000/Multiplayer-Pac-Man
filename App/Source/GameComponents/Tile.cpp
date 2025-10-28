#include "Tile.h"
#include "Core/Application.h"

Tile::Tile(Type type, Vector2Ex<int> position, Vector2Ex<int> dimensions)
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

void Tile::Render(Vector2Ex<int> offset) const
{
    Vector2Ex<int> position = GetPosition();
    Vector2Ex<int> size = GetDimensions();
    std::shared_ptr<Texture2D> pathTexture = Core::Application::GetTexturesManager()->GetTexture("path");
    std::shared_ptr<Texture2D> wallTexture = Core::Application::GetTexturesManager()->GetTexture("wall");

    if (m_type == Type::Wall)
    {
        DrawTextureEx(*wallTexture, position, 0, (float)size.y / (float)wallTexture->height, WHITE);
    }
    else if (m_type == Type::Path)
    {
        DrawTextureEx(*pathTexture, position, 0, (float)size.y / (float)pathTexture->height, WHITE);
    }
}
