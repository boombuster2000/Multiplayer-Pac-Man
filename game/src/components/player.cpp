#include "game/components/player.h"

int Player::GetPoints() const
{
    return m_points;
}

void Player::AddPoints(const int pointsToAdd)
{
    m_points += pointsToAdd;
}
