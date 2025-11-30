#include "game/components/ghost.h"

Ghost::Ghost(const Vector2Ex<float>& spawnPosition,
             const Vector2Ex<float>& speed,
             const Vector2Ex<float>& dimensions,
             const ui::Direction& direction,
             const std::shared_ptr<Texture2D>& texture) :
    Entity(spawnPosition, speed, dimensions, direction, texture)
{
}