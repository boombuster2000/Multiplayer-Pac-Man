#include "GameLayer.h"

#include <array>
#include <iostream>
#include <stdexcept>

#include "Core/Application.h"
#include "Core/InputManager.h"
#include "GameOptionsMenuLayer/GameOptionsMenuLayer.h"
#include "MainMenuLayer/MainMenuLayer.h"
#include "raylib.h"

bool GameLayer::CanMoveInDirection(const Vector2Ex<float> &position, const UIComponents::Direction &direction) const
{
    using namespace UIComponents;
    const Vector2Ex<float> pacmanSize = m_pacman.GetDimensions() - Vector2Ex<float>(1, 1);

    Vector2Ex<float> cornersToCheck[2];
    switch (direction)
    {
    case Direction::UP:
        cornersToCheck[0] = position;
        cornersToCheck[1] = position + Vector2Ex<float>(pacmanSize.x, 0);
        break;
    case Direction::DOWN:
        cornersToCheck[0] = position + Vector2Ex<float>(0, pacmanSize.y);
        cornersToCheck[1] = position + Vector2Ex<float>(pacmanSize.x, pacmanSize.y);
        break;
    case Direction::LEFT:
        cornersToCheck[0] = position;
        cornersToCheck[1] = position + Vector2Ex<float>(0, pacmanSize.y);
        break;
    case Direction::RIGHT:
        cornersToCheck[0] = position + Vector2Ex<float>(pacmanSize.x, 0);
        cornersToCheck[1] = position + Vector2Ex<float>(pacmanSize.x, pacmanSize.y);
        break;
    }

    for (auto &corner : cornersToCheck)
    {
        const Tile &tile = m_board.GetTileFromPosition(corner);
        if (tile.GetType() == Tile::Type::Wall)
            return false;
    }

    return true;
}

Vector2Ex<float> GameLayer::GetNextValidPacmanPosition(Vector2Ex<float> start, Vector2Ex<float> end,
                                                       UIComponents::Direction direction)
{
    using namespace UIComponents;
    const Vector2Ex<float> movementDelta = end - start;

    // Number of intermediate steps to check
    float steps = (direction == Direction::UP || direction == Direction::DOWN) ? std::abs(movementDelta.y)
                                                                               : std::abs(movementDelta.x);

    Vector2Ex<float> nextValidPosition = start;

    // Allow direction change when stationary (no movement yet)
    UIComponents::Direction queuedDir = m_pacman.GetQueuedDirection();

    if (IsPacmanTouchingPellet(m_pacman.GetDimensions(), nextValidPosition))
    {
        Tile &tile = m_board.GetTileFromPosition(nextValidPosition);
        Pellet &pellet = tile.GetPellet();

        int pointsGained = pellet.GetValue();

        pellet.SetType(Pellet::Type::NONE);
    }

    if (queuedDir != direction)
    {
        const Vector2Ex<float> queuedVec = Vector2Ex<float>::GetDirectionVector(queuedDir);
        const Vector2Ex<float> peekPosition = start + queuedVec;

        if (CanMoveInDirection(peekPosition, queuedDir))
        {
            m_pacman.ApplyQueuedDirection();
            direction = queuedDir;

            float moveDistance = steps;
            end = start + (queuedVec * moveDistance);

            return GetNextValidPacmanPosition(start, end, direction);
        }
    }

    if (steps <= 0)
        return start;

    float remaining_steps = steps;

    while (remaining_steps > 0)
    {
        // The incremental step size (1 or less for the last step)
        float step_size = std::min(remaining_steps, 1.0f);

        // Current total step taken
        float current_total_step = steps - remaining_steps + step_size;

        // Calculate the intermediate position
        Vector2Ex<float> intermediatePosition = start + (movementDelta / steps) * current_total_step;

        if (IsPacmanTouchingPellet(m_pacman.GetDimensions(), intermediatePosition))
        {
            Tile &tile = m_board.GetTileFromPosition(intermediatePosition);
            Pellet &pellet = tile.GetPellet();

            int pointsGained = pellet.GetValue();

            pellet.SetType(Pellet::Type::NONE);
        }

        // Check if queued direction is possible here
        UIComponents::Direction queuedDir = m_pacman.GetQueuedDirection();
        if (queuedDir != direction)
        {
            Vector2Ex<float> peekPosition = intermediatePosition + Vector2Ex<float>::GetDirectionVector(queuedDir);
            if (CanMoveInDirection(peekPosition, queuedDir))
            {
                m_pacman.ApplyQueuedDirection();
                direction = queuedDir;

                start = intermediatePosition;

                // Redirect remaining movement distance in new direction
                end = start + (Vector2Ex<float>::GetDirectionVector(direction) * remaining_steps);

                return GetNextValidPacmanPosition(start, end, direction);
            }
        }

        // Regular collision check
        if (!CanMoveInDirection(intermediatePosition, direction))
            return nextValidPosition;

        nextValidPosition = intermediatePosition;
        remaining_steps -= step_size;
    }

    return nextValidPosition;
}

bool GameLayer::IsPacmanTouchingPellet(const Vector2Ex<float> &pacmanDimensions,
                                       const Vector2Ex<float> &pacmanPosition) const
{
    const Tile &tile = m_board.GetTileFromPosition(pacmanPosition);
    const Pellet &pellet = tile.GetPellet();
    const Vector2Ex<float> pelletPosition = pellet.GetPositionAtAnchor();
    const Vector2Ex<float> pelletDimensions = pellet.GetDimensions();
    const Rectangle pacmanRec = {pacmanPosition.x, pacmanPosition.y, pacmanDimensions.x, pacmanDimensions.y};
    const Rectangle pelletRec = {pelletPosition.x, pelletPosition.y, pelletDimensions.x, pelletDimensions.y};

    return CheckCollisionRecs(pacmanRec, pelletRec);
}

GameLayer::GameLayer()
    : m_board(Board()), m_pacman(m_board.GetPositionFromIndex(Vector2Ex<int>(1, 1)), Vector2Ex<float>(50, 50), 400)
{
}

GameLayer::GameLayer(const std::string &boardPath)
    : m_board(Board(boardPath)),
      m_pacman(m_board.GetPositionFromIndex(Vector2Ex<int>(1, 1)), Vector2Ex<float>(50, 50), 400)
{
}

void GameLayer::HandleKeyPresses()
{
    using enum UIComponents::Direction;
    auto inputManager = Core::Application::GetInputManager();

    if (inputManager->IsAction("move_up", Core::InputState::PRESSED))
        m_pacman.QueueDirection(UP);

    if (inputManager->IsAction("move_down", Core::InputState::PRESSED))
        m_pacman.QueueDirection(DOWN);

    if (inputManager->IsAction("move_left", Core::InputState::PRESSED))
        m_pacman.QueueDirection(LEFT);

    if (inputManager->IsAction("move_right", Core::InputState::PRESSED))
        m_pacman.QueueDirection(RIGHT);

    if (IsKeyPressed(KEY_F1))
        m_board.SaveToFile("./Boards/default.json");

    if (inputManager->IsAction("pause", Core::InputState::PRESSED))
    {
        SuspendUpdate();
        Push(std::make_unique<GameOptionsMenuLayer>());
    }
}

void GameLayer::HandleCollisions(const float &deltaTime)
{
    const Vector2Ex<float> positionBeforeMove = m_pacman.GetPositionAtAnchor();
    const Vector2Ex<float> nextPosition = m_pacman.GetNextPosition(m_pacman.GetCurrentDirection(), deltaTime);

    const Vector2Ex<float> newPosition =
        GetNextValidPacmanPosition(positionBeforeMove, nextPosition, m_pacman.GetCurrentDirection());
    m_pacman.SetPosition(newPosition);
}

void GameLayer::OnUpdate(float ts)
{
    HandleKeyPresses();
    HandleCollisions(ts);
}

void GameLayer::OnRender()
{
    m_board.Render();
    m_pacman.Render();
}
