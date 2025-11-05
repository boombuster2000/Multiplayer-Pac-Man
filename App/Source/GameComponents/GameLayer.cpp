#include "GameLayer.h"
#include "raylib.h"
#include "Core/Application.h"
#include "DataTypes/Bounds.h"

#include <iostream>
#include <stdexcept>
#include <array>

Vector2Ex<int> GameLayer::GetNextValidPacmanPosition(const UIComponents::Direction direction) const
{
    using namespace UIComponents;
    using enum Direction;

    const Vector2Ex<int> currentPosition = m_pacman.GetPositionAtAnchor();
    const Vector2Ex<int> nextPosition = m_pacman.GetNextPosition(direction);

    const Vector2Ex<int> movementDelta = nextPosition - currentPosition;

    int steps;

    if (direction == UP || direction == DOWN)
        steps = std::abs(movementDelta.y);
    else
        steps = std::abs(movementDelta.x);

    Vector2Ex<int> nextValidPosition = currentPosition;
    for (int step = 0; step <= steps; step++)
    {
        Vector2Ex<int> intermediatePosition;
        if (steps == 0)
            intermediatePosition = currentPosition;
        else
            intermediatePosition = currentPosition + (movementDelta / steps) * step;

        Vector2Ex<int> cornersToCheck[2];
        const Vector2Ex<int> pacmanDimensions = m_pacman.GetDimensions();

        switch (direction)
        {
        case UP:
            cornersToCheck[0] = intermediatePosition;
            cornersToCheck[1] = intermediatePosition + Vector2Ex<int>(pacmanDimensions.x, 0);
            break;

        case DOWN:
            cornersToCheck[0] = intermediatePosition + Vector2Ex<int>(0, pacmanDimensions.y);
            cornersToCheck[1] = intermediatePosition + Vector2Ex<int>(pacmanDimensions.x, pacmanDimensions.y);
            break;

        case LEFT:
            cornersToCheck[0] = intermediatePosition;
            cornersToCheck[1] = intermediatePosition + Vector2Ex<int>(0, pacmanDimensions.y);
            break;

        case RIGHT:
            cornersToCheck[0] = intermediatePosition + Vector2Ex<int>(pacmanDimensions.x, 0);
            cornersToCheck[1] = intermediatePosition + Vector2Ex<int>(pacmanDimensions.x, pacmanDimensions.y);
            break;
        }

        bool hasHitWall = false;
        for (auto corner : cornersToCheck)
        {
            // This is done because the tile top right corner position is the exact same as the top left corner position of the next tile.
            if (direction == LEFT)
                corner.x -= 1;
            else if (direction == UP)
                corner.y -= 1;

            const Tile &tileAtCorner = m_board.GetTileFromPosition(corner);
            if (tileAtCorner.GetType() == Tile::Type::Wall)
            {
                hasHitWall = true;
                break;
            }
        }

        if (hasHitWall)
            return nextValidPosition;
        else
            nextValidPosition = intermediatePosition;
    }

    return nextValidPosition;
}

GameLayer::GameLayer()
    : m_board(Board()),
      m_pacman(m_board.GetPositionFromIndex(Vector2Ex<int>(1, 1)) + Vector2Ex<int>(1, 1), Vector2Ex<int>(48, 48), 5)
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

    const Vector2Ex<int> currentPosition = m_pacman.GetPositionAtAnchor();
    const Vector2Ex<int> nextQueuedPosition = GetNextValidPacmanPosition(m_pacman.GetQueuedDirection());

    if (currentPosition != nextQueuedPosition)
        m_pacman.ApplyQueuedDirection();

    m_pacman.SetPosition(GetNextValidPacmanPosition(m_pacman.GetCurrentDirection()));
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
