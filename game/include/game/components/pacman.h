#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "game/components/entity.h"
#include "raylib.h"
#include <memory>

class Pacman : public Entity
{

  private:
    Vector2Ex<float> m_lastPosition;
    int m_lives = 3;
    bool m_isDead = false;
    float m_respawnTimer = 0.0f;

  private:
    float ConvertDirectionToRotation(const ui::Direction& direction) const;

  public:
    Pacman(Vector2Ex<float> spawnPosition,
           Vector2Ex<float> dimensions,
           float speed,
           Color color = Color(255, 239, 0, 255));

    void SetPosition(const Vector2Ex<float> position) final;
    void SetQueuedDirection(const ui::Direction& direction) final;
    void ApplyQueuedDirection() final;

    Vector2Ex<float> GetLastPosition() const;

    bool IsStationary() const;

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