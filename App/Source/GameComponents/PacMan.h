#include "DataTypes/Vector2Ex.h"
#include "UIComponents/Enums.h"
#include "UIComponents/RenderableObject.h"
#include "raylib.h"
#include <memory>

class PacMan : public UIComponents::RenderableObject
{
  private:
    using enum UIComponents::Direction;
    std::shared_ptr<Texture2D> m_texture;
    Vector2Ex<float> m_spawnPosition;
    Vector2Ex<float> m_dimensions;
    float m_speed;
    float m_rotation;
    Vector2Ex<float> m_lastPosition;
    UIComponents::Direction m_currentDirection;
    UIComponents::Direction m_queuedDirection;

  private:
    void SetRotation(const UIComponents::Direction &direction)
    {
        switch (direction)
        {
        case UP:
            m_rotation = -90;
            break;
        case DOWN:
            m_rotation = 90;
            break;
        case LEFT:
            m_rotation = 180;
            break;
        case RIGHT:
            m_rotation = 0;
            break;
        default:
            break;
        }
    }

  public:
    PacMan(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed);
    ~PacMan() = default;

    UIComponents::Direction GetCurrentDirection() const;
    UIComponents::Direction GetQueuedDirection() const;

    void QueueDirection(UIComponents::Direction direction);
    void ApplyQueuedDirection();

    void SetPosition(const Vector2Ex<float> position);
    Vector2Ex<float> GetNextPosition(const UIComponents::Direction &direction, const float &deltaTime) const;
    Vector2Ex<float> GetNextPositionWithStep(const UIComponents::Direction &direction, const float step) const;

    float GetSpeed() const;

    bool IsStationary() const
    {
        return GetWorldOrigin() == m_lastPosition;
    }

    virtual Vector2Ex<float> GetDimensions() const override;
    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
};