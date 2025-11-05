#include "PacMan.h"
#include "Core/Application.h"

PacMan::PacMan(Vector2Ex<int> spawnPosition, Vector2Ex<int> dimensions, float speed)
    : RenderableObject(spawnPosition), m_texture(Core::Application::GetTexturesManager()->GetTexture("pac-man")),
      m_spawnPosition(spawnPosition), m_dimensions(dimensions), m_speed(speed), m_currentDirection(LEFT), m_queuedDirection(m_currentDirection)
{
}

UIComponents::Direction PacMan::GetCurrentDirection() const
{
    return m_currentDirection;
}

UIComponents::Direction PacMan::GetQueuedDirection() const
{
    return m_queuedDirection;
}

void PacMan::QueueDirection(UIComponents::Direction direction)
{
    m_queuedDirection = direction;
}

void PacMan::ApplyQueuedDirection()
{
    m_currentDirection = m_queuedDirection;
}

void PacMan::SetPosition(const Vector2Ex<int> position)
{
    RenderableObject::SetPosition(position);
}

void PacMan::UpdatePosition()
{
    Move(m_currentDirection, m_speed);
}

Vector2Ex<int> PacMan::GetNextPosition(UIComponents::Direction direction) const
{
    using enum UIComponents::Direction;
    Vector2Ex<int> nextPosition = GetPositionAtAnchor();

    switch (direction)
    {
    case UP:
        nextPosition.y -= m_speed;
        break;

    case DOWN:
        nextPosition.y += m_speed;
        break;

    case LEFT:
        nextPosition.x -= m_speed;
        break;

    case RIGHT:
        nextPosition.x += m_speed;
        break;

    default:
        break;
    }

    return nextPosition;
}

Vector2Ex<int> PacMan::GetDimensions() const
{
    return m_dimensions;
}

void PacMan::Render(Vector2Ex<int> offset) const
{
    using enum UIComponents::Direction;
    using enum UIComponents::AnchorPoint;

    const float scale = static_cast<float>(m_dimensions.y) / static_cast<float>(m_texture->height);
    int rotation = 0;

    switch (m_currentDirection)
    {
    case UP:
        rotation = -90;
        break;
    case DOWN:
        rotation = 90;
        break;
    case LEFT:
        rotation = 180;
        break;
    case RIGHT:
        rotation = 0;
        break;
    }

    const Vector2Ex<int> centerPosition = GetPositionAtAnchor(MIDDLE);

    // Define source and destination rectangles
    Rectangle srcRect = {0.0f, 0.0f, static_cast<float>(m_texture->width), static_cast<float>(m_texture->height)};
    Rectangle destRect = {
        static_cast<float>(centerPosition.x + offset.x),
        static_cast<float>(centerPosition.y + offset.y),
        m_texture->width * scale,
        m_texture->height * scale};

    // Center of rotation (pivot)
    Vector2 origin = {(m_texture->width * scale) / 2.0f, (m_texture->height * scale) / 2.0f};

    DrawTexturePro(*m_texture, srcRect, destRect, origin, rotation, WHITE);
}
