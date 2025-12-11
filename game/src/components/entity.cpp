#include "game/components/entity.h"

Entity::Entity(const EntityType entityType,
               const Vector2Ex<float>& spawnPosition,
               const Vector2Ex<float>& speed,
               const Vector2Ex<float>& dimensions,
               const ui::Direction& direction,
               const std::shared_ptr<Texture2D>& texture,
               const Color& color) :
    RenderableObject(spawnPosition),
    m_entityType(entityType),
    m_spawnPosition(spawnPosition),
    m_speed(speed),
    m_dimensions(dimensions),
    m_direction(direction),
    m_queuedDirection(direction),
    m_texture(texture),
    m_color(color)
{
}

EntityType Entity::GetEntityType() const
{
    return m_entityType;
}

Vector2Ex<float> Entity::GetSpawnPosition() const
{
    return m_spawnPosition;
}

void Entity::SetSpawnPosition(const Vector2Ex<float>& position)
{
    m_spawnPosition = position;
}

Vector2Ex<float> Entity::GetSpeed() const
{
    return m_speed;
}

void Entity::SetSpeed(const Vector2Ex<float>& speed)
{
    m_speed = speed;
}

float Entity::GetRotation() const
{
    return m_rotation;
}

void Entity::SetRotation(const float& rotation)
{
    m_rotation = rotation;
}

Color Entity::GetColor() const
{
    return m_color;
}
void Entity::SetColor(const Color& color)
{
    m_color = color;
}

ui::Direction Entity::GetDirection() const
{
    return m_direction;
}

void Entity::SetDirection(const ui::Direction& direction)
{
    m_direction = direction;
}

ui::Direction Entity::GetQueuedDirection() const
{
    return m_queuedDirection;
}

void Entity::SetQueuedDirection(const ui::Direction& direction)
{
    m_queuedDirection = direction;
}

void Entity::ApplyQueuedDirection()
{
    m_direction = m_queuedDirection;
}

Vector2Ex<float> Entity::GetDimensions() const
{
    return m_dimensions;
}

void Entity::SetDimensions(const Vector2Ex<float>& dimensions)
{
    m_dimensions = dimensions;
}

std::shared_ptr<Texture2D> Entity::GetTexture() const
{
    return m_texture;
}

void Entity::SetTexture(const std::shared_ptr<Texture2D>& texture)
{
    m_texture = texture;
}
void Entity::SetPosition(const Vector2Ex<float> position)
{
    m_lastPosition = GetPositionAtAnchor();
    ui::RenderableObject::SetPosition(position);
}

Vector2Ex<float> Entity::GetNextPosition(const ui::Direction& direction, const float deltaTime) const
{
    using enum ui::Direction;
    Vector2Ex<float> nextPosition = GetPositionAtAnchor();

    switch (direction)
    {
    case UP:
        nextPosition.y -= m_speed.y * deltaTime;
        break;

    case DOWN:
        nextPosition.y += m_speed.y * deltaTime;
        break;

    case LEFT:
        nextPosition.x -= m_speed.x * deltaTime;
        break;

    case RIGHT:
        nextPosition.x += m_speed.x * deltaTime;
        break;

    default:
        break;
    }

    return nextPosition;
}

Vector2Ex<float> Entity::GetNextPositionWithStep(const ui::Direction& direction, const float step) const
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
bool Entity::IsStationary() const
{
    return m_lastPosition == GetPositionAtAnchor();
}

Vector2Ex<float> Entity::GetLastPosition() const
{
    return m_lastPosition;
}

void Entity::Render(Vector2Ex<float> offset) const
{
    using enum ui::Direction;
    using enum ui::AnchorPoint;

    const float& textureWidth = static_cast<float>(m_texture->width);
    const float& textureHeight = static_cast<float>(m_texture->height);

    const float scaleX = m_dimensions.x / textureWidth;
    const float scaleY = m_dimensions.y / textureHeight;

    const Vector2Ex<float> centerPosition = GetPositionAtAnchor(MIDDLE);

    Rectangle sourceRect = {0.0f, 0.0f, textureWidth, textureHeight};
    Rectangle destRect = {centerPosition.x + offset.x,
                          centerPosition.y + offset.y,
                          textureWidth * scaleX,
                          textureHeight * scaleY};

    // Center of rotation (pivot)
    Vector2 originPosition = {(textureWidth * scaleX) / 2.0f, (textureHeight * scaleY) / 2.0f};

    DrawTexturePro(*m_texture, sourceRect, destRect, originPosition, m_rotation, m_color);
}
