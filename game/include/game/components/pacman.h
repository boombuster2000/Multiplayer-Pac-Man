#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "engine/ui/renderable_object.h"
#include "raylib.h"
#include <memory>

class Pacman : public ui::RenderableObject
{
  private:
    using enum ui::Direction;
    std::shared_ptr<Texture2D> m_texture;
    Vector2Ex<float> m_spawnPosition;
    Vector2Ex<float> m_dimensions;
    float m_speed;
    float m_Rotation;
    Vector2Ex<float> m_lastPosition;
    ui::Direction m_currentDirection;
    ui::Direction m_queuedDirection;

  private:
    void SetRotation(const ui::Direction& direction);

  public:
    Pacman();
    Pacman(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float Speed);

    ui::Direction GetCurrentDirection() const;
    ui::Direction GetQueuedDirection() const;

    void QueueDirection(ui::Direction direction);
    void ApplyQueuedDirection();

    void SetPosition(const Vector2Ex<float> position) final;
    Vector2Ex<float> GetNextPosition(const ui::Direction& direction, const float& deltaTime) const;
    Vector2Ex<float> GetNextPositionWithStep(const ui::Direction& direction, const float step) const;

    float GetSpeed() const;

    bool IsStationary() const;

    Vector2Ex<float> GetDimensions() const final;
    void SetDimensions(const Vector2Ex<float>& dimensions);

    void Render(Vector2Ex<float> offset = {0, 0}) const final;
};