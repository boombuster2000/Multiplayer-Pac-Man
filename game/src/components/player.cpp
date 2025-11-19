#include "game/components/player.h"
#include "game/components/profile.h"

Player::Player(std::shared_ptr<Profile> profile, Pacman pacman) : m_profile(profile), m_pacman(pacman), m_points(0)
{
}

void Player::SetProfile(std::shared_ptr<Profile> profile)
{
    m_profile = profile;
}

int Player::GetPoints() const
{
    return m_points;
}

void Player::AddPoints(const int pointsToAdd)
{
    m_points += pointsToAdd;
}

Pacman& Player::GetPacman()
{
    return m_pacman;
}

const Pacman& Player::GetPacman() const
{
    return m_pacman;
}
