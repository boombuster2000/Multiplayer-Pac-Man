#include "game/components/pacman.h"
#include "game/game_application.h"

float Pacman::ConvertDirectionToRotation(const ui::Direction& direction) const
{
    using enum ui::Direction;
    switch (direction)
    {
    case UP:
        return 270.0f;
    case DOWN:
        return 90.0f;
    case LEFT:
        return 180.0f;
    case RIGHT:
        return 0.0f;
    default:
        return 0.0f;
    }
}

Pacman::Pacman(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed) :
    Entity(EntityType::PACMAN,
           spawnPosition,
           Vector2Ex<float>(speed, speed),
           dimensions,
           ui::Direction::LEFT,
           game::GameApplication::Get().GetTexturesManager().GetTexture("pac-man"))
{
}

void Pacman::SetPosition(const Vector2Ex<float> position)
{
    m_lastPosition = GetWorldOrigin();
    ui::RenderableObject::SetPosition(position);
}

void Pacman::SetQueuedDirection(const ui::Direction& direction)
{
    Entity::SetQueuedDirection(direction);
    if (IsStationary())
        SetRotation(ConvertDirectionToRotation(GetQueuedDirection()));
}

void Pacman::ApplyQueuedDirection()
{
    Entity::ApplyQueuedDirection();
    SetRotation(ConvertDirectionToRotation(GetDirection()));
}

Vector2Ex<float> Pacman::GetLastPosition() const
{
    return m_lastPosition;
}

bool Pacman::IsStationary() const
{
    return GetWorldOrigin() == m_lastPosition;
}