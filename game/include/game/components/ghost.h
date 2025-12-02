#include "board.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "entity.h"
#include "game/components/pacman.h"


class Ghost : public Entity
{
  private:
  public:
    Ghost(const Vector2Ex<float>& spawnPosition,
          const Vector2Ex<float>& speed,
          const Vector2Ex<float>& dimensions,
          const ui::Direction& direction,
          const std::shared_ptr<Texture2D>& texture);

    virtual ~Ghost() = default;

    virtual void UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition) = 0;
};