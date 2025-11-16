#include "Tile.h"
#include "Core/Application.h"

Tile::Tile()
    : GridTile(), m_type(Type::None),
      m_pellet(GetPositionAtAnchor(UIComponents::AnchorPoint::MIDDLE), Pellet::Type::NONE)
{
}

Tile::Tile(Type type, Vector2Ex<float> position, Vector2Ex<float> dimensions)
    : GridTile(dimensions, position), m_type(type),
      m_pellet(GetPositionAtAnchor(UIComponents::AnchorPoint::MIDDLE), Pellet::Type::NONE)
{
}

Tile::Tile(Type type, Pellet::Type pelletType, Vector2Ex<float> position, Vector2Ex<float> dimensions)
    : GridTile(dimensions, position), m_type(type), m_pellet(GetPositionAtAnchor(UIComponents::AnchorPoint::MIDDLE),
                                                             type == Type::Wall ? Pellet::Type::NONE : pelletType)
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

Pellet &Tile::GetPellet()
{
    return m_pellet;
}

const Pellet &Tile::GetPellet() const
{
    return m_pellet;
}

void Tile::SetPosition(const Vector2Ex<float> &position)
{
    RenderableObject::SetPosition(position);
    m_pellet.SetPosition(GetPositionAtAnchor(UIComponents::AnchorPoint::MIDDLE));
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
        m_pellet.Render();
    }
}
