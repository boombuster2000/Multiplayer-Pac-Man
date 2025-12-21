#include "game/components/tile.h"
#include "game/game_application.h"

Tile::Tile() :
    GridTile(),
    m_type(Type::NONE),
    m_pellet(GetPositionAtAnchor(ui::AnchorPoint::MIDDLE), Pellet::Type::NONE)
{
}

Tile::Tile(Type type, Vector2Ex<float> position, Vector2Ex<float> dimensions) :
    GridTile(dimensions, position),
    m_type(type),
    m_pellet(GetPositionAtAnchor(ui::AnchorPoint::MIDDLE), Pellet::Type::NONE)
{
}

Tile::Tile(Type type, Pellet::Type pelletType, Vector2Ex<float> position, Vector2Ex<float> dimensions) :
    GridTile(dimensions, position),
    m_type(type),
    m_pellet(GetPositionAtAnchor(ui::AnchorPoint::MIDDLE), type == Type::WALL ? Pellet::Type::NONE : pelletType)
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

Pellet& Tile::GetPellet()
{
    return m_pellet;
}

const Pellet& Tile::GetPellet() const
{
    return m_pellet;
}

void Tile::SetPosition(Vector2Ex<float> position)
{
    RenderableObject::SetPosition(position);
    m_pellet.SetPosition(GetPositionAtAnchor(ui::AnchorPoint::MIDDLE));
}

void Tile::Render(Vector2Ex<float> offset) const
{
    Vector2Ex<float> position = GetPositionAtAnchor();
    Vector2Ex<float> size = GetDimensions();
    std::shared_ptr<Texture2D> pathTexture = engine::Application::GetTexturesManager().GetTexture("path");
    std::shared_ptr<Texture2D> wallTexture = engine::Application::GetTexturesManager().GetTexture("wall");

    if (m_type == Type::WALL)
    {
        Rectangle srcRect = {0, 0, (float)wallTexture->width, (float)wallTexture->height};
        Rectangle destRect = {position.x, position.y, size.x, size.y};
        DrawTexturePro(*wallTexture, srcRect, destRect, {0, 0}, 0, WHITE);
    }
    else if (m_type == Type::PATH)
    {
        Rectangle srcRect = {0, 0, (float)pathTexture->width, (float)pathTexture->height};
        Rectangle destRect = {position.x, position.y, size.x, size.y};
        DrawTexturePro(*pathTexture, srcRect, destRect, {0, 0}, 0, WHITE);
        m_pellet.Render();
    }
}
