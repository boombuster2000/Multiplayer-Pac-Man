#include "game/components/player.h"

int Player::GetPoints() const
{
    return m_points;
}

void Player::AddPoints(const int pointsToAdd)
{
    m_points += pointsToAdd;
}

int Player::GetLives() const
{
    return m_lives;
}

void Player::SetLives(const int lives)
{
    m_lives = lives;
}

void Player::RemoveLife(const int livesToRemove)
{
    m_lives -= livesToRemove;
}

void Player::AddLife(const int livesToAdd)
{
    m_lives += livesToAdd;
}

bool Player::IsDead() const
{
    return m_isDead;
}

void Player::SetDead(const bool isDead)
{
    m_isDead = isDead;
}

float Player::GetRespawnTimer() const
{
    return m_respawnTimer;
}

void Player::SetRespawnTimer(const float time)
{
    m_respawnTimer = time;
}

void Player::UpdateRespawnTimer(const float dt)
{
    if (m_respawnTimer > 0)
    {
        m_respawnTimer -= dt;
    }
}
