#pragma once
#include "pacman.h"
#include "profile.h"
#include <memory>
#include <string>

class Player
{
  private:
    int m_points;
    std::shared_ptr<Profile> m_profile;
    Pacman m_pacman;

  public:
    Player(std::shared_ptr<Profile> profile, Pacman pacman);

    void SetProfile(std::shared_ptr<Profile> profile);

    int GetPoints() const;
    void AddPoints(const int pointsToAdd);

    Pacman& GetPacman();
    const Pacman& GetPacman() const;
};