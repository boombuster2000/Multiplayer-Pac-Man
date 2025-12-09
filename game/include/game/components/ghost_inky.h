#pragma once
#include "board.h"
#include "ghost.h"

class Inky : public Ghost
{
  public:
    Inky(const Vector2Ex<float>& spawnPosition,
         const Vector2Ex<float>& speed,
         const Vector2Ex<float>& dimensions,
         const ui::Direction& direction,
         const State state = State::SPAWNING);

    void Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection) final;
};