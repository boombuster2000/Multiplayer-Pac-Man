#include "game/components/pacman.h"
#include "engine/core/application.h"

Pacman::Pacman() = default;

void Pacman::SetRotation(const ui::Direction& direction)
{
    switch (direction)
    {
    case UP:
        m_Rotation = -90;
        break;
    case DOWN:
        m_Rotation = 90;
        break;
    case LEFT:
        m_Rotation = 180;
        break;
    case RIGHT:
        m_Rotation = 0;
        break;
    default:
        break;
    }
}

Pacman::Pacman(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed)
    : RenderableObject(spawnPosition), m_texture(engine::Application::GetTexturesManager()->GetTexture("pac-man")),
      m_spawnPosition(spawnPosition), m_dimensions(dimensions), m_speed(speed), m_Rotation(0),
      m_lastPosition(spawnPosition), m_currentDirection(LEFT), m_queuedDirection(m_currentDirection)
{
}

ui::Direction Pacman::GetCurrentDirection() const
{
    return m_currentDirection;
}

ui::Direction Pacman::GetQueuedDirection() const
{
    return m_queuedDirection;
}

void Pacman::QueueDirection(ui::Direction direction)
{
    m_queuedDirection = direction;

    if (IsStationary())
        SetRotation(m_queuedDirection);
}

void Pacman::ApplyQueuedDirection()
{
    m_currentDirection = m_queuedDirection;
    SetRotation(m_currentDirection);
}

void Pacman::SetPosition(const Vector2Ex<float> position)
{
    m_lastPosition = GetWorldOrigin();
    RenderableObject::SetPosition(position);
}

Vector2Ex<float> Pacman::GetNextPosition(const ui::Direction& direction, const float& deltaTime) const
{
    using enum ui::Direction;
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

Vector2Ex<float> Pacman::GetNextPositionWithStep(const ui::Direction& direction, const float step) const
{
    using enum ui::Direction;
    Vector2Ex<float> nextPosition = GetPositionAtAnchor();

    switch (direction)
    {
    case UP:
        nextPosition.y -= step;
        break;

    case DOWN:
        nextPosition.y += step;
        break;

    case LEFT:
        nextPosition.x -= step;
        break;

    case RIGHT:
        nextPosition.x += step;
        break;

    default:
        break;
    }

    return nextPosition;
}

float Pacman::GetSpeed() const
{
    return m_speed;
}

bool Pacman::IsStationary() const
{
    return GetWorldOrigin() == m_lastPosition;
}

Vector2Ex<float> Pacman::GetDimensions() const
{
    return m_dimensions;
}

void Pacman::SetDimensions(const Vector2Ex<float>& dimensions)
{
    m_dimensions = dimensions;
}

void Pacman::Render(Vector2Ex<float> offset) const
{
    using enum ui::Direction;
    using enum ui::AnchorPoint;

    const float scale = m_dimensions.y / static_cast<float>(m_texture->height);

    const Vector2Ex<float> centerPosition = GetPositionAtAnchor(MIDDLE);

    // Define source and destination rectangles

    const float& textureWidth = static_cast<float>(m_texture->width);
    const float& textureHeight = static_cast<float>(m_texture->height);

    Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
    Rectangle destRect = {centerPosition.x + offset.x, centerPosition.y + offset.y, m_texture->width * scale,
                          m_texture->height * scale};

    // Center of rotation (pivot)
    Vector2 origin = {(textureWidth * scale) / 2.0f, (textureHeight * scale) / 2.0f};

    DrawTexturePro(*m_texture, sourceRect, destRect, origin, m_Rotation, WHITE);
}
