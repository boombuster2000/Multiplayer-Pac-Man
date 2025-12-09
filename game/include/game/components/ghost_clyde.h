#pragma once
#include "board.h"
#include "ghost.h"

class Clyde : public Ghost
{
  private:
  public:
    Clyde(const Vector2Ex<float>& spawnPosition,
          const Vector2Ex<float>& speed,
          const Vector2Ex<float>& dimensions,
          const ui::Direction& direction);

    void Update(const Board& board, const Vector2Ex<float>& pacmanPosition, const ui::Direction pacmanDirection) final;
};