#include "PacMan.h"
#include "Core/Application.h"

PacMan::PacMan(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed)
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

void PacMan::SetPosition(const Vector2Ex<float> position)
{
    RenderableObject::SetPosition(position);
}

void PacMan::UpdatePosition()
{
    Move(m_currentDirection, m_speed);
}

Vector2Ex<float> PacMan::GetNextPosition(const UIComponents::Direction &direction, const float &deltaTime) const
{
    using enum UIComponents::Direction;
    Vector2Ex<float> nextPosition = GetPositionAtAnchor();

    switch (direction)
    {
    case UP:
        nextPosition.y -= m_speed * deltaTime;
        break;

    case DOWN:
        nextPosition.y += m_speed * deltaTime;
        break;

    case LEFT:
        nextPosition.x -= m_speed * deltaTime;
        break;

    case RIGHT:
        nextPosition.x += m_speed * deltaTime;
        break;

    default:
        break;
    }

    return nextPosition;
}

float PacMan::GetSpeed() const
{
    return m_speed;
}

Vector2Ex<float> PacMan::GetDimensions() const
{
    return m_dimensions;
}

void PacMan::Render(Vector2Ex<float> offset) const
{
    using enum UIComponents::Direction;
    using enum UIComponents::AnchorPoint;

    const float scale = m_dimensions.y / static_cast<float>(m_texture->height);
    float rotation = 0;

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

    const Vector2Ex<float> centerPosition = GetPositionAtAnchor(MIDDLE);

    // Define source and destination rectangles

    const float &textureWidth = static_cast<float>(m_texture->width);
    const float &textureHeight = static_cast<float>(m_texture->height);

    Rectangle srcRect = {0.0f, 0.0f, textureWidth, textureHeight};
    Rectangle destRect = {
        centerPosition.x + offset.x,
        centerPosition.y + offset.y,
        m_texture->width * scale,
        m_texture->height * scale};

    // Center of rotation (pivot)
    Vector2 origin = {(textureWidth * scale) / 2.0f, (textureHeight * scale) / 2.0f};

    DrawTexturePro(*m_texture, srcRect, destRect, origin, rotation, WHITE);
}
