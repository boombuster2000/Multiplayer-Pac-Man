#include "game/components/ghost_inky.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/game_application.h"

Inky::Inky(const Vector2Ex<float>& spawnPosition,
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
          Ghost::Type::INKY,
          guardPosition,
          releaseTime,
          game::GameApplication::Get().GetTexturesManager().GetTexture("inky"),
          state)
{
}

void Inky::Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection)
{
    // Same behavior as blinky
    UpdateQueuedDirection(board, pacmanPosition);
}