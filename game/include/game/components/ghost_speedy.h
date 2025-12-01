#include "ghost.h"

class Speedy : public Ghost
{
  private:
  public:
    Speedy(const Vector2Ex<float>& spawnPosition,
           const Vector2Ex<float>& speed,
           const Vector2Ex<float>& dimensions,
           const ui::Direction& direction,
           const std::shared_ptr<Texture2D>& texture);

    void UpdateQueuedDirection(const Vector2Ex<float>& targetPosition) final;
};