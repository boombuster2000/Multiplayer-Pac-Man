#include "GameLayer.h"
#include "raylib.h"
#include "Core/Application.h"
#include "DataTypes/Bounds.h"

#include <iostream>
#include <stdexcept>
#include <array>

bool GameLayer::ShouldPacmanMove(const UIComponents::Direction direction) const
{
    using namespace UIComponents;
    using enum Direction;

    const Vector2Ex<int> nextPosition = m_pacman.GetNextPosition(direction);

    const int offset = 1; // Small offset to check corners within the hitbox
    Vector2Ex<int> cornersToCheck[2];

    switch (direction)
    {
    case UP:
        cornersToCheck[0] = nextPosition + Vector2Ex<int>(offset, offset);
        cornersToCheck[1] = nextPosition + Vector2Ex<int>(m_pacman.GetDimensions().x, 0) + Vector2Ex<int>(-offset, offset);
        break;

    case DOWN:
        cornersToCheck[0] = nextPosition + Vector2Ex<int>(0, m_pacman.GetDimensions().y) + Vector2Ex<int>(offset, -offset);
        cornersToCheck[1] = nextPosition + m_pacman.GetDimensions() + Vector2Ex<int>(-offset, -offset);
        break;

    case LEFT:
        cornersToCheck[0] = nextPosition + Vector2Ex<int>(1, 1);
        cornersToCheck[1] = nextPosition + Vector2Ex<int>(0, m_pacman.GetDimensions().y) + Vector2Ex<int>(offset, -offset);
        break;

    case RIGHT:
        cornersToCheck[0] = nextPosition + Vector2Ex<int>(m_pacman.GetDimensions().x, 0) + Vector2Ex<int>(-offset, offset);
        cornersToCheck[1] = nextPosition + m_pacman.GetDimensions() + Vector2Ex<int>(-offset, -offset);
        break;
    }

    for (const auto &corner : cornersToCheck)
    {
        const Tile &tileAtCorner = m_board.GetTileFromPosition(corner); // Slight offset to ensure we are within the tile
        if (tileAtCorner.GetType() == Tile::Type::Wall)
            return false;
    }

    return true;
}

GameLayer::GameLayer()
    : m_board(Board()),
      m_pacman(m_board.GetPositionFromIndex(Vector2Ex<int>(1, 1)), Vector2Ex<int>(50, 50), 5)
{
}

void GameLayer::HandleKeyPresses()
{
    using enum UIComponents::Direction;
    if (IsKeyPressed(KEY_W))
        m_pacman.QueueDirection(UP);

    if (IsKeyPressed(KEY_S))
        m_pacman.QueueDirection(DOWN);

    if (IsKeyPressed(KEY_A))
        m_pacman.QueueDirection(LEFT);

    if (IsKeyPressed(KEY_D))
        m_pacman.QueueDirection(RIGHT);
}

void GameLayer::HandleCollisions()
{

    if (ShouldPacmanMove(m_pacman.GetQueuedDirection()))
        m_pacman.ApplyQueuedDirection();

    if (ShouldPacmanMove(m_pacman.GetCurrentDirection()))
        m_pacman.UpdatePosition();
}

void GameLayer::OnUpdate(float ts)
{
    HandleKeyPresses();
    HandleCollisions();
}

void GameLayer::OnRender()
{
    ClearBackground(WHITE);
    m_board.Render();
    m_pacman.Render();
}
