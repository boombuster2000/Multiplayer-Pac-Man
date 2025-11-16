#include "GameLayer.h"

#include <array>
#include <iostream>
#include <stdexcept>

#include "Core/Application.h"
#include "Core/InputManager.h"
#include "GameOptionsMenuLayer/GameOptionsMenuLayer.h"
#include "MainMenuLayer/MainMenuLayer.h"
#include "raylib.h"

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

void GameLayer::CollectPelletAtPosition(const Vector2Ex<float> &position)
{
    if (IsPacmanTouchingPellet(m_pacman.GetDimensions(), position))
    {
        Tile &tile = m_board.GetTileFromPosition(position);
        Pellet &pellet = tile.GetPellet();

        int pointsGained = pellet.GetValue();

        pellet.SetType(Pellet::Type::NONE);
    }
}

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

bool GameLayer::TryApplyQueuedDirection(Vector2Ex<float> &currentPosition, UIComponents::Direction &currentDirection)
{
    UIComponents::Direction queuedDir = m_pacman.GetQueuedDirection();

    if (queuedDir == currentDirection)
        return false;

    const Vector2Ex<float> queuedVec = Vector2Ex<float>::GetDirectionVector(queuedDir);
    const Vector2Ex<float> peekPosition = currentPosition + queuedVec;

    if (CanMoveInDirection(peekPosition, queuedDir))
    {
        m_pacman.ApplyQueuedDirection();
        currentDirection = queuedDir;
        return true;
    }

    return false;
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
    using namespace UIComponents;

    Vector2Ex<float> currentPosition = m_pacman.GetPositionAtAnchor();
    Direction currentDirection = m_pacman.GetCurrentDirection();

    // Collect pellet at starting position
    CollectPelletAtPosition(currentPosition);

    // Calculate desired movement
    const Vector2Ex<float> targetPosition = m_pacman.GetNextPosition(currentDirection, deltaTime);
    const Vector2Ex<float> movementDelta = targetPosition - currentPosition;

    // Number of intermediate steps to check
    float totalDistance = (currentDirection == Direction::UP || currentDirection == Direction::DOWN)
                              ? std::abs(movementDelta.y)
                              : std::abs(movementDelta.x);

    // Try to apply queued direction at start if stationary or at current position
    float remainingDistance = totalDistance;
    if (TryApplyQueuedDirection(currentPosition, currentDirection))
    {
        // Direction changed, update current direction for the pacman
        currentDirection = m_pacman.GetCurrentDirection();
    }

    if (totalDistance <= 0)
    {
        m_pacman.SetPosition(currentPosition);
        return;
    }

    Vector2Ex<float> lastValidPosition = currentPosition;

    // Loop through intermediate positions
    while (remainingDistance > 0)
    {
        // The incremental step size (1 or less for the last step)
        float stepSize = std::min(remainingDistance, 1.0f);

        // Calculate the intermediate position using current direction
        Vector2Ex<float> directionVector = Vector2Ex<float>::GetDirectionVector(currentDirection);
        Vector2Ex<float> intermediatePosition = currentPosition + (directionVector * stepSize);

        // Try to apply queued direction at this position
        if (TryApplyQueuedDirection(intermediatePosition, currentDirection))
        {
            CollectPelletAtPosition(intermediatePosition);

            // Direction changed, update position and direction, then continue in new direction
            lastValidPosition = intermediatePosition;
            currentPosition = intermediatePosition;
            currentDirection = m_pacman.GetCurrentDirection();
            continue;
        }

        // Check collision
        if (!CanMoveInDirection(intermediatePosition, currentDirection))
        {
            // Hit a wall, stop at last valid position
            break;
        }

        // Position is valid, collect pellets and continue
        CollectPelletAtPosition(intermediatePosition);
        lastValidPosition = intermediatePosition;
        currentPosition = intermediatePosition;
        remainingDistance -= stepSize;
    }

    m_pacman.SetPosition(lastValidPosition);
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
