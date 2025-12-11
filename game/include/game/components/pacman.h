#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "game/components/entity.h"
#include "raylib.h"

class Pacman : public Entity
{

private:
    int m_lives = 3;
    bool m_isDead = false;
    float m_respawnTimer = 0.0f;

  private:
    static float ConvertDirectionToRotation(const ui::Direction& direction);

  public:
    Pacman(Vector2Ex<float> spawnPosition,
           Vector2Ex<float> dimensions,
           float speed,
           Color color = Color(255, 239, 0, 255));

    void SetQueuedDirection(const ui::Direction& direction) final;
    void ApplyQueuedDirection() final;

    [[nodiscard]] int GetLives() const;
    void SetLives(int lives);
    void RemoveLife(int livesToRemove = 1);
    void AddLife(int livesToAdd = 1);

    [[nodiscard]] bool IsDead() const;
    void SetDead(bool isDead);

    [[nodiscard]] float GetRespawnTimer() const;
    void SetRespawnTimer(float time);
    void UpdateRespawnTimer(float dt);
};