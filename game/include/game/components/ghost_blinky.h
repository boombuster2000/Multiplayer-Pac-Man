#include "board.h"
#include "ghost.h"

class Blinky : public Ghost
{
  private:
  public:
    Blinky(const Vector2Ex<float>& spawnPosition,
           const Vector2Ex<float>& speed,
           const Vector2Ex<float>& dimensions,
           const ui::Direction& direction);

    void UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition) final;
};