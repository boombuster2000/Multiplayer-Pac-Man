#include "game/components/ghost_pinky.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/enums.h"
#include "game/components/node_system.h"
#include "game/game_application.h"
#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

Pinky::Pinky(const Vector2Ex<float>& spawnPosition,
             const Vector2Ex<float>& speed,
             const Vector2Ex<float>& dimensions,
             const ui::Direction& direction) :
    Ghost(spawnPosition,
          speed,
          dimensions,
          direction,
          game::GameApplication::Get().GetTexturesManager().GetTexture("pinky"))
{
}

void Pinky::Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection)
{
    UpdateQueuedDirection(board, pacmanPosition);
}