#include "game/components/ghost_blinky.h"
#include "game/game_application.h"

Blinky::Blinky(const Vector2Ex<float>& spawnPosition,
               const Vector2Ex<float>& speed,
               const Vector2Ex<float>& dimensions,
               const ui::Direction& direction) :
    Ghost(spawnPosition,
          speed,
          dimensions,
          direction,
          game::GameApplication::Get().GetTexturesManager().GetTexture("blinky"))
{
}

void Blinky::UpdateQueuedDirection(const Vector2Ex<float>& targetPosition)
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