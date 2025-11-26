#include "game/layers/game.h"

#include <array>
#include <iostream>
#include <stdexcept>

#include "engine/core/input_manager.h"
#include "game/game_application.h"
#include "game/layers/game_options_menu.h"
#include "game/layers/main_menu.h"
#include "raylib.h"
#include <array>
#include <string>

bool GameLayer::IsPacmanTouchingPellet(const Vector2Ex<float>& pacmanDimensions,
                                       const Vector2Ex<float>& pacmanPosition) const
{
    const Tile& tile = m_board.GetTileFromPosition(pacmanPosition);
    const Pellet& pellet = tile.GetPellet();
    const Vector2Ex<float> pelletPosition = pellet.GetPositionAtAnchor();
    const Vector2Ex<float> pelletDimensions = pellet.GetDimensions();
    const Rectangle pacmanRec = {pacmanPosition.x, pacmanPosition.y, pacmanDimensions.x, pacmanDimensions.y};
    const Rectangle pelletRec = {pelletPosition.x, pelletPosition.y, pelletDimensions.x, pelletDimensions.y};

    return CheckCollisionRecs(pacmanRec, pelletRec);
}

void GameLayer::CollectPelletAtPosition(const Vector2Ex<float>& position)
{
    if (IsPacmanTouchingPellet(m_player.GetPacman().GetDimensions(), position))
    {
        Tile& tile = m_board.GetTileFromPosition(position);
        Pellet& pellet = tile.GetPellet();

        int pointsGained = pellet.GetValue();
        m_player.AddPoints(pointsGained);
        UpdateHighscores();

        pellet.SetType(Pellet::Type::NONE);
    }
}

bool GameLayer::CanMoveInDirection(const Vector2Ex<float>& position, const ui::Direction& direction) const
{
    using namespace ui;
    using enum AnchorPoint;
    using enum Direction;
    const Vector2Ex<float> pacmanDimensions = m_player.GetPacman().GetDimensions();

    std::array<Vector2Ex<float>, 2> cornersToCheck;

    switch (direction)
    {
    case UP:
        cornersToCheck[0] = position;
        cornersToCheck[1] = position + RenderableObject::CalculateAnchorOffset(TOP_RIGHT, pacmanDimensions);
        break;
    case DOWN:
        cornersToCheck[0] = position + RenderableObject::CalculateAnchorOffset(BOTTOM_LEFT, pacmanDimensions);
        cornersToCheck[1] = position + RenderableObject::CalculateAnchorOffset(BOTTOM_RIGHT, pacmanDimensions);
        break;
    case LEFT:
        cornersToCheck[0] = position;
        cornersToCheck[1] = position + RenderableObject::CalculateAnchorOffset(BOTTOM_LEFT, pacmanDimensions);
        break;
    case RIGHT:
        cornersToCheck[0] = position + RenderableObject::CalculateAnchorOffset(TOP_RIGHT, pacmanDimensions);
        cornersToCheck[1] = position + RenderableObject::CalculateAnchorOffset(BOTTOM_RIGHT, pacmanDimensions);
        break;
    }

    for (auto& corner : cornersToCheck)
    {
        const Tile& tile = m_board.GetTileFromPosition(corner);
        if (tile.GetType() == Tile::Type::WALL)
            return false;
    }

    return true;
}

bool GameLayer::TryApplyQueuedDirection(Vector2Ex<float>& currentPosition, ui::Direction& currentDirection)
{
    ui::Direction queuedDir = m_player.GetPacman().GetQueuedDirection();

    if (queuedDir == currentDirection)
        return false;

    const Vector2Ex<float> queuedVec = Vector2Ex<float>::GetDirectionVector(queuedDir);
    const Vector2Ex<float> peekPosition = currentPosition + queuedVec;

    if (CanMoveInDirection(peekPosition, queuedDir))
    {
        m_player.GetPacman().ApplyQueuedDirection();
        currentDirection = queuedDir;
        return true;
    }

    return false;
}

GameLayer::GameLayer()
    : m_board(), m_player(game::GameApplication::Get().GetProfile(),
                          Pacman(m_board.GetPlayerSpawnPoint(), Vector2Ex<float>(50, 50), 400))
{
}

GameLayer::GameLayer(const std::string& boardPath)
    : m_board(boardPath), m_player(game::GameApplication::Get().GetProfile(),
                                   Pacman(m_board.GetPlayerSpawnPoint(), Vector2Ex<float>(50, 50), 400))
{
}

GameLayer::~GameLayer()
{
    m_board.SaveHighscoresToFile();
}

void GameLayer::HandleKeyPresses()
{
    using enum ui::Direction;
    auto inputManager = game::GameApplication::GetInputManager();

    if (inputManager.IsAction("move_up", engine::InputState::PRESSED))
        m_player.GetPacman().QueueDirection(UP);

    if (inputManager.IsAction("move_down", engine::InputState::PRESSED))
        m_player.GetPacman().QueueDirection(DOWN);

    if (inputManager.IsAction("move_left", engine::InputState::PRESSED))
        m_player.GetPacman().QueueDirection(LEFT);

    if (inputManager.IsAction("move_right", engine::InputState::PRESSED))
        m_player.GetPacman().QueueDirection(RIGHT);

    if (IsKeyPressed(KEY_F1))
        m_board.SaveToFile();

    if (inputManager.IsAction("pause", engine::InputState::PRESSED))
    {
        SuspendUpdate();
        Push(std::make_unique<GameOptionsMenuLayer>());
    }
}

void GameLayer::HandleCollisions(const float& deltaTime)
{
    using namespace ui;

    Vector2Ex<float> currentPosition = m_player.GetPacman().GetPositionAtAnchor();
    Direction currentDirection = m_player.GetPacman().GetCurrentDirection();

    // Collect pellet at starting position
    CollectPelletAtPosition(currentPosition);

    // Calculate desired movement
    const Vector2Ex<float> targetPosition = m_player.GetPacman().GetNextPosition(currentDirection, deltaTime);
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
        currentDirection = m_player.GetPacman().GetCurrentDirection();
    }

    if (totalDistance <= 0)
    {
        m_player.GetPacman().SetPosition(currentPosition);
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
            currentDirection = m_player.GetPacman().GetCurrentDirection();
            continue;
        }

        // Check collision
        if (!CanMoveInDirection(intermediatePosition, currentDirection))
        {
            // Hit a wall, stop at last valid position
            break;
        }

        // position is valid, collect pellets and continue
        CollectPelletAtPosition(intermediatePosition);
        lastValidPosition = intermediatePosition;
        currentPosition = intermediatePosition;
        remainingDistance -= stepSize;
    }

    m_player.GetPacman().SetPosition(lastValidPosition);
}

void GameLayer::UpdateHighscores()
{
    const std::string& boardName = m_board.GetName();

    game::GameApplication::Get().GetProfile()->UpdateHighScore(boardName, m_player.GetPoints());
    m_board.SetHighscore(game::GameApplication::Get().GetProfile()->GetUsername(), m_player.GetPoints());
}

void GameLayer::OnUpdate(float ts)
{
    m_timePassedSinceLastSave += ts;

    if (m_timePassedSinceLastSave >= 10.0f)
    {
        m_board.SaveHighscoresToFile();
        m_timePassedSinceLastSave = 0.0f;
    }

    HandleKeyPresses();
    HandleCollisions(ts);
}

void GameLayer::OnRender()
{
    m_board.Render();
    m_player.GetPacman().Render();
    RenderScores();
}

void GameLayer::RenderScores()
{
    const int currentPoints = m_player.GetPoints();
    const std::string& boardName = m_board.GetName();
    const auto& highscores = game::GameApplication::Get().GetProfile()->GetPersonalHighscores();

    int highscore = 0;
    if (highscores.contains(boardName))
    {
        highscore = highscores.at(boardName);
    }

    const std::string currentPointsStr = "Score: " + std::to_string(currentPoints);
    const std::string highscoreStr = "Highscore: " + std::to_string(highscore);

    DrawText(highscoreStr.c_str(), 10, 10, 20, BLACK);
    DrawText(currentPointsStr.c_str(), 10, 40, 20, BLACK);
}
