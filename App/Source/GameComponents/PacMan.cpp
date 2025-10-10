#include "PacMan.h"
#include "Core/Application.h"

PacMan::PacMan(Vector2 spawnPosition, Vector2 size, float speed)
:RenderableObject(spawnPosition), m_spawnPosition(spawnPosition), m_size(size), m_speed(speed),
m_texture(Core::Application::GetTexturesManager()->GetTexture("pac-man")), m_queuedDirection(LEFT), m_currentDirection(LEFT)
{
    UpdateDrawPoint();
}

void PacMan::QueueDirection(UIComponents::Direction direction)
{
    m_queuedDirection = direction;
}

void PacMan::Update()
{

    m_currentDirection = m_queuedDirection;
    Move(m_currentDirection, m_speed);
}

Vector2 PacMan::GetSize() const
{
    return m_size;
}

void PacMan::Render() const
{
    float scale = m_size.y/m_texture->height;
    DrawTextureEx(*m_texture, GetPosition(), 0, scale, WHITE);
}
