#include "raylib.h"
#include "UIComponents/RenderableObject.h"
#include <memory>

class PacMan : public UIComponents::RenderableObject
{
    private:
    std::shared_ptr<Texture2D> m_texture;
    Vector2 m_spawnPosition;
    Vector2 m_size;

    public:
    PacMan(Vector2 spawnPosition, Vector2 size);
    ~PacMan() = default;

    virtual Vector2 GetSize() const override;
    virtual void Render() const override;

};