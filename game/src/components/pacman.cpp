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

Pacman::Pacman(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed, Color color) :
    Entity(EntityType::PACMAN,
           spawnPosition,
           Vector2Ex<float>(speed, speed),
           dimensions,
           ui::Direction::RIGHT,
           game::GameApplication::Get().GetTexturesManager().GetTexture("pac-man"),
           color)
{
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

int Pacman::GetLives() const
{
    return m_lives;
}

void Pacman::SetLives(const int lives)
{
    m_lives = lives;
}

void Pacman::RemoveLife(const int livesToRemove)
{
    m_lives -= livesToRemove;
}

void Pacman::AddLife(const int livesToAdd)
{
    m_lives += livesToAdd;
}

bool Pacman::IsDead() const
{
    return m_isDead;
}

void Pacman::SetDead(const bool isDead)
{
    m_isDead = isDead;
}

float Pacman::GetRespawnTimer() const
{
    return m_respawnTimer;
}

void Pacman::SetRespawnTimer(const float time)
{
    m_respawnTimer = time;
}

void Pacman::UpdateRespawnTimer(const float dt)
{
    if (m_respawnTimer <= 0)
        return;

    m_respawnTimer -= dt;
}
