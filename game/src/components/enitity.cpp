#include "game/components/entity.h"

Entity::Entity(const Vector2Ex<float>& spawnPosition,
               const Vector2Ex<float>& speed,
               const Vector2Ex<float>& dimensions,
               const ui::Direction& direction,
               const std::shared_ptr<Texture2D>& texture) :
    RenderableObject(spawnPosition),
    m_spawnPosition(spawnPosition),
    m_speed(speed),
    m_dimensions(dimensions),
    m_direction(direction),
    m_queuedDirection(direction),
    m_texture(texture)
{
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

    DrawTexturePro(*m_texture, sourceRect, destRect, originPosition, m_rotation, WHITE);
}
