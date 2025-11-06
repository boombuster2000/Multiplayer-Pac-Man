#include <memory>
#include "raylib.h"
#include "UIComponents/RenderableObject.h"
#include "UIComponents/Enums.h"
#include "DataTypes/Vector2Ex.h"

class PacMan : public UIComponents::RenderableObject
{
private:
    using enum UIComponents::Direction;
    std::shared_ptr<Texture2D> m_texture;
    Vector2Ex<float> m_spawnPosition;
    Vector2Ex<float> m_dimensions;
    float m_speed;
    UIComponents::Direction m_currentDirection;
    UIComponents::Direction m_queuedDirection;

public:
    PacMan(Vector2Ex<float> spawnPosition, Vector2Ex<float> dimensions, float speed);
    ~PacMan() = default;

    UIComponents::Direction GetCurrentDirection() const;
    UIComponents::Direction GetQueuedDirection() const;

    void QueueDirection(UIComponents::Direction direction);
    void ApplyQueuedDirection();

    void SetPosition(const Vector2Ex<float> position);
    void UpdatePosition();
    Vector2Ex<float> GetNextPosition(const UIComponents::Direction &direction, const float &deltaTime) const;

    virtual Vector2Ex<float> GetDimensions() const override;
    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
};