#include "GameLayer.h"
#include "raylib.h"
#include "Core/Application.h"
#include "DataTypes/Bounds.h"

#include <iostream>
#include <stdexcept>
#include <array>

Vector2Ex<float> GameLayer::GetNextValidPacmanPosition(const Vector2Ex<float> &start, const Vector2Ex<float> &end, const UIComponents::Direction &direction) const
{
    using namespace UIComponents;
    using enum Direction;

    const Vector2Ex<float> movementDelta = end - start;

    float steps;

    if (direction == UP || direction == DOWN)
        steps = std::abs(movementDelta.y);
    else
        steps = std::abs(movementDelta.x);

    Vector2Ex<float> nextValidPosition = start;
    if (steps > 0)
    {
        float remaining_steps = steps;
        while (remaining_steps > 0)
        {
            float step_size = std::min(remaining_steps, 1.0f);
            float current_total_step = steps - remaining_steps + step_size;

            Vector2Ex<float> intermediatePosition = start + (movementDelta / steps) * current_total_step;

            Vector2Ex<float> cornersToCheck[2];
            const Vector2Ex<float> pacmanDimensions = m_pacman.GetDimensions() - Vector2Ex<float>(1, 1); // The pixel after is the next tile.

            switch (direction)
            {
            case UP:
                cornersToCheck[0] = intermediatePosition;
                cornersToCheck[1] = intermediatePosition + Vector2Ex<float>(pacmanDimensions.x, 0);
                break;

            case DOWN:
                cornersToCheck[0] = intermediatePosition + Vector2Ex<float>(0, pacmanDimensions.y);
                cornersToCheck[1] = intermediatePosition + Vector2Ex<float>(pacmanDimensions.x, pacmanDimensions.y);
                break;

            case LEFT:
                cornersToCheck[0] = intermediatePosition;
                cornersToCheck[1] = intermediatePosition + Vector2Ex<float>(0, pacmanDimensions.y);
                break;

            case RIGHT:
                cornersToCheck[0] = intermediatePosition + Vector2Ex<float>(pacmanDimensions.x, 0);
                cornersToCheck[1] = intermediatePosition + Vector2Ex<float>(pacmanDimensions.x, pacmanDimensions.y);
                break;
            }

            bool hasHitWall = false;
            for (auto corner : cornersToCheck)
            {

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

            remaining_steps -= step_size;
        }
    }

    return nextValidPosition;
}

GameLayer::GameLayer()
    : m_board(Board()),
      m_pacman(m_board.GetPositionFromIndex(Vector2Ex<int>(1, 1)), Vector2Ex<float>(50, 50), 400)
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

void GameLayer::HandleCollisions(const float &deltaTime)
{

    const Vector2Ex<float> currentPosition = m_pacman.GetPositionAtAnchor();
    const Vector2Ex<float> nextPosition = m_pacman.GetNextPosition(m_pacman.GetCurrentDirection(), deltaTime);
    const Vector2Ex<float> nextQueuedPosition = m_pacman.GetNextPositionWithStep(m_pacman.GetQueuedDirection(), 1.0f);
    const Vector2Ex<float> validNextQueuedPosition = GetNextValidPacmanPosition(currentPosition, nextQueuedPosition, m_pacman.GetQueuedDirection());

    if (currentPosition != validNextQueuedPosition)
        m_pacman.ApplyQueuedDirection();

    const Vector2Ex<float> validNextPosition = GetNextValidPacmanPosition(currentPosition, nextPosition, m_pacman.GetCurrentDirection());
    m_pacman.SetPosition(validNextPosition);
}

void GameLayer::OnUpdate(float ts)
{

    HandleKeyPresses();
    HandleCollisions(ts);
}

void GameLayer::OnRender()
{
    ClearBackground(WHITE);
    m_board.Render();
    m_pacman.Render();
}
