#include "game/components/ghost_clyde.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/game_application.h"

Clyde::Clyde(const Vector2Ex<float>& spawnPosition,
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
          Ghost::Type::CLYDE,
          guardPosition,
          releaseTime,
          game::GameApplication::Get().GetTexturesManager().GetTexture("clyde"),
          state)
{
}

void Clyde::Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection)
{
    // Will chase pacman if distance to pacman is within 4 tiles.
    const Vector2Ex<float>& tileDimensions = board.GetTileDimensions();
    constexpr float tilesToTriggerChase = 4;
    const float distanceToTriggerChase = tileDimensions.x * tilesToTriggerChase; // Assuming tile is square.

    // Need to update direction so that it is towards the next node to target
    UpdateQueuedDirection(board, pacmanPosition);

    const float distanceToTarget =
        board.GetDirectedDistanceThroughNodes(GetPositionAtAnchor(), GetDirection(), pacmanPosition);

    if (distanceToTarget > distanceToTriggerChase) // if target is too far just go to guard position
    {
        SetState(Ghost::State::SCATTER); // Will be synced to main ghost mode next frame by game.
        UpdateQueuedDirection(board, GetGuardPosition());
    }
}