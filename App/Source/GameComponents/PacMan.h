#include <memory>
#include "raylib.h"
#include "UIComponents/RenderableObject.h"
#include "UIComponents/Enums.h"

class PacMan : public UIComponents::RenderableObject
{
    private:
    using enum UIComponents::Direction;
    std::shared_ptr<Texture2D> m_texture;
    Vector2 m_spawnPosition;
    Vector2 m_size;
    float m_speed;
    UIComponents::Direction m_currentDirection;


    public:
    PacMan(Vector2 spawnPosition, Vector2 size, float speed);
    ~PacMan() = default;

    void SetDirection(UIComponents::Direction direction);
    void Update();

    virtual Vector2 GetSize() const override;
    virtual void Render() const override;

};