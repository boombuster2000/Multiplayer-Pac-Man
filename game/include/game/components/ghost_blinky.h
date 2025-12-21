#pragma once
#include "ghost.h"

class Blinky : public Ghost
{
  public:
    explicit Blinky(const Vector2Ex<float>& spawnPosition,
                    const Vector2Ex<float>& speed,
                    const Vector2Ex<float>& dimensions,
                    const ui::Direction& direction,
                    const Type ghostType,
                    const Vector2Ex<float>& guardPosition,
                    const float releaseTime,
                    const State state = State::SPAWNING);

    void Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection);
};