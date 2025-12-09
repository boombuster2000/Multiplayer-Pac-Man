#pragma once

class Player
{
  private:
    int m_points = 0;
    int m_lives = 3;
    bool m_isDead = false;
    float m_respawnTimer = 0.0f;

  public:
    Player() = default;

    int GetPoints() const;
    void AddPoints(const int pointsToAdd);

    int GetLives() const;
    void SetLives(const int lives);
    void RemoveLife(const int livesToRemove = 1);
    void AddLife(const int livesToAdd = 1);

    bool IsDead() const;
    void SetDead(const bool isDead);

    float GetRespawnTimer() const;
    void SetRespawnTimer(const float time);
    void UpdateRespawnTimer(const float dt);
};