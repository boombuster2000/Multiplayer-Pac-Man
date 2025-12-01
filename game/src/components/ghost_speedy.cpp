#include "game/components/ghost_speedy.h"

Speedy::Speedy(const Vector2Ex<float>& spawnPosition,
               const Vector2Ex<float>& speed,
               const Vector2Ex<float>& dimensions,
               const ui::Direction& direction,
               const std::shared_ptr<Texture2D>& texture) :
    Ghost(spawnPosition, speed, dimensions, direction, texture)
{
}

void Speedy::UpdateQueuedDirection(const Vector2Ex<float>& targetPosition)
{
    Vector2Ex<float> currentPosition = GetPositionAtAnchor();
    Vector2Ex<float> directionToTarget = targetPosition - currentPosition;

    if (std::abs(directionToTarget.x) > std::abs(directionToTarget.y))
    {
        if (directionToTarget.x > 0)
            SetQueuedDirection(ui::Direction::RIGHT);
        else
            SetQueuedDirection(ui::Direction::LEFT);
    }
    else
    {
        if (directionToTarget.y > 0)
            SetQueuedDirection(ui::Direction::DOWN);
        else
            SetQueuedDirection(ui::Direction::UP);
    }
}