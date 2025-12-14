#include "game/components/ghost_pinky.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/game_application.h"

Pinky::Pinky(const Vector2Ex<float>& spawnPosition,
             const Vector2Ex<float>& speed,
             const Vector2Ex<float>& dimensions,
             const ui::Direction& direction,
             const Type ghostType,
             const Vector2Ex<float>& guardPosition,
             const float releaseTime,
             const State state) :
    Ghost(spawnPosition,
          speed,
          dimensions,
          direction,
          Ghost::Type::PINKY,
          guardPosition,
          releaseTime,
          game::GameApplication::Get().GetTexturesManager().GetTexture("pinky"),
          state)
{
}

void Pinky::Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection)
{
    Vector2Ex<float> targetPosition;

    // If Pinky is in front of Pac-Man, target Pac-Man directly.
    if (board.HasLineOfSight(GetPositionAtAnchor(), targetPosition))
    {
        targetPosition = pacmanPosition;
    }
    else
    {
        // Original logic: Targets 4 tiles ahead of pacman
        const Vector2Ex<float> tileDimensions = board.GetTileDimensions();
        const Vector2Ex<float> directionVector = Vector2Ex<float>::GetDirectionVector(pacmanDirection);
        constexpr float tilesAhead = 4;

        const Vector2Ex<float> offset = {tileDimensions.x * directionVector.x * tilesAhead,
                                         tileDimensions.y * directionVector.y * tilesAhead};
        targetPosition = pacmanPosition + offset;
    }

    UpdateQueuedDirection(board, targetPosition);
}