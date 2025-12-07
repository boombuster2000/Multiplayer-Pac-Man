#pragma once

class Player
{
  private:
    int m_points = 0;

  public:
    Player() = default;

    int GetPoints() const;
    void AddPoints(const int pointsToAdd);
};