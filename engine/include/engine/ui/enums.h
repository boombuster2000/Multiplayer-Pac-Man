#pragma once

namespace ui
{
enum class AnchorPoint : int
{
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,

    MIDDLE_LEFT,
    MIDDLE,
    MIDDLE_RIGHT,

    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

enum class Alignment : int
{
    LEFT,
    CENTER,
    RIGHT
};

enum class Direction : int
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

inline ui::Direction GetOppositeDirection(const ui::Direction dir)
{
    using enum ui::Direction;
    switch(dir)
    {
    case UP:    return DOWN;
    case DOWN:  return UP;
    case LEFT:  return RIGHT;
    case RIGHT: return LEFT;
    default:    return UP; // Should be unreachable
    }
}

inline bool IsOppositeDirection(const ui::Direction current, const ui::Direction proposed)
{
    using enum ui::Direction;
    return (current == UP && proposed == DOWN) ||
           (current == DOWN && proposed == UP) ||
           (current == LEFT && proposed == RIGHT) ||
           (current == RIGHT && proposed == LEFT);
}
} // namespace ui
