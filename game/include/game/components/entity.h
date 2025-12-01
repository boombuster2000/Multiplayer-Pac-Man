#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "engine/ui/renderable_object.h"
#include "raylib.h"
#include <memory>

class Entity : public ui::RenderableObject
{
    Vector2Ex<float> m_spawnPosition;
    Vector2Ex<float> m_speed;
    Vector2Ex<float> m_dimensions;
    ui::Direction m_direction;
    ui::Direction m_queuedDirection;
    std::shared_ptr<Texture2D> m_texture;
    float m_rotation = 0.0f;

  private:
    Entity() = default;

  public:
    Entity(const Vector2Ex<float>& spawnPosition,
           const Vector2Ex<float>& speed,
           const Vector2Ex<float>& dimensions,
           const ui::Direction& direction,
           const std::shared_ptr<Texture2D>& texture);

    Vector2Ex<float> GetSpawnPosition() const;
    void SetSpawnPosition(const Vector2Ex<float>& position);

    Vector2Ex<float> GetSpeed() const;
    void SetSpeed(const Vector2Ex<float>& speed);

    float GetRotation() const;
    void SetRotation(const float& rotation);

    ui::Direction GetDirection() const;
    void SetDirection(const ui::Direction& direction);

    ui::Direction GetQueuedDirection() const;
    virtual void SetQueuedDirection(const ui::Direction& direction);
    virtual void ApplyQueuedDirection();

    Vector2Ex<float> GetDimensions() const final;
    void SetDimensions(const Vector2Ex<float>& dimensions);

    std::shared_ptr<Texture2D> GetTexture() const;
    void SetTexture(const std::shared_ptr<Texture2D>& texture);

    Vector2Ex<float> GetNextPosition(const ui::Direction& direction, const float deltaTime) const;
    Vector2Ex<float> GetNextPositionWithStep(const ui::Direction& direction, const float step) const;

    void Render(Vector2Ex<float> offset = {0, 0}) const final;
};