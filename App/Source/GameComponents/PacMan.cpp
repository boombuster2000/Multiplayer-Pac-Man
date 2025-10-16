#include "PacMan.h"
#include "Core/Application.h"

PacMan::PacMan(Vector2Ex<int> spawnPosition, Vector2Ex<int> dimensions, float speed)
    : RenderableObject(spawnPosition), m_spawnPosition(spawnPosition), m_dimensions(dimensions), m_speed(speed),
      m_texture(Core::Application::GetTexturesManager()->GetTexture("pac-man")), m_currentDirection(LEFT)
{
    UpdateDrawPoint();
}

void PacMan::SetDirection(UIComponents::Direction direction)
{
    m_currentDirection = direction;
}

void PacMan::Update()
{
    Move(m_currentDirection, m_speed);
}

Vector2Ex<int> PacMan::GetSize() const
{
    return m_dimensions;
}

void PacMan::Render(Vector2Ex<int> offset) const
{
    const float scale = (float)m_dimensions.y / (float)m_texture->height;
    DrawTextureEx(*m_texture, GetPosition(), 0, scale, WHITE);
}
