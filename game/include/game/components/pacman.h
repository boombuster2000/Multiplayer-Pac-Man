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

  private:
    float ConvertDirectionToRotation(const ui::Direction& direction) const;

  public:
    Pacman(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed);

    void SetPosition(const Vector2Ex<float> position) final;
    void SetQueuedDirection(const ui::Direction& direction) final;
    void ApplyQueuedDirection() final;

    bool IsStationary() const;
};