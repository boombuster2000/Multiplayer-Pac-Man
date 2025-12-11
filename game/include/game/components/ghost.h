#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "entity.h"
#include "game/components/pacman.h"

class Board;

class Ghost : public Entity
{
    private:
    static bool IsOppositeDirection(ui::Direction current, ui::Direction proposed);

    public:
    enum class State
    {
        SCATTER,
        CHASE,
        FRIGHTENED,
        SPAWNING,
        DEAD
    };

    enum class Type
    {
        BLINKY,
        PINKY,
        INKY,
        CLYDE
    };

  private:
    State m_state;
    Type m_ghostType;

    Vector2Ex<float> m_guardPosition;
    float m_releaseTime;

  public:
    explicit Ghost(const Vector2Ex<float>& spawnPosition,
                   const Vector2Ex<float>& speed,
                   const Vector2Ex<float>& dimensions,
                   const ui::Direction& direction,
                   const Type ghostType,
                   const Vector2Ex<float>& guardPosition,
                   const float releaseTime,
                   const std::shared_ptr<Texture2D>& texture,
                   const State state = State::SPAWNING);

    ~Ghost() override = default;

    State GetState() const;
    void SetState(const State state);

    Type GetType() const;

    float GetReleaseTime() const;
    void SetReleaseTime(const float releaseTime);

    Vector2Ex<float> GetGuardPosition() const;
    void SetGuardPosition(const Vector2Ex<float>& guardPosition);

    void UpdateQueuedDirection(const Board& board, const Vector2Ex<float>& targetPosition);
    virtual void Update(const Board& board,
                        const Vector2Ex<float>& pacmanPosition,
                        const ui::Direction pacmanDirection) = 0;
};