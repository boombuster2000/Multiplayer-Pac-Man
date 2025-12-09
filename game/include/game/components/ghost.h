#pragma once
#include "board.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "entity.h"
#include "game/components/pacman.h"

class Ghost : public Entity
{
  protected:
    void UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition);

  public:
    enum class State
    {
        SCATTER,
        CHASE,
        FRIGHTENED,
        SPAWNING,
        DEAD
    };

  private:
    State m_state;

  public:
    Ghost(const Vector2Ex<float>& spawnPosition,
          const Vector2Ex<float>& speed,
          const Vector2Ex<float>& dimensions,
          const ui::Direction& direction,
          const std::shared_ptr<Texture2D>& texture,
          const State state = State::SPAWNING);

    virtual ~Ghost() = default;

    State GetState() const;
    void SetState(const State state);

    virtual void Update(const Board& board,
                        const Vector2Ex<float>& pacmanPosition,
                        const ui::Direction pacmanDirection) = 0;
};