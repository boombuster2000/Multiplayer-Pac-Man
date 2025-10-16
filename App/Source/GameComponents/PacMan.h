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
    Vector2Ex<int> m_spawnPosition;
    Vector2Ex<int> m_dimensions;
    float m_speed;
    UIComponents::Direction m_currentDirection;

public:
    PacMan(Vector2Ex<int> spawnPosition, Vector2Ex<int> dimensions, float speed);
    ~PacMan() = default;

    void SetDirection(UIComponents::Direction direction);
    void Update();

    virtual Vector2Ex<int> GetSize() const override;
    virtual void Render(Vector2Ex<int> offset = {0, 0}) const override;
};