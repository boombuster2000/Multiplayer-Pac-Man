#include "game/layers/game.h"
#include "engine/core/input_manager.h"
#include "game/game_application.h"
#include "game/layers/game_options_menu.h"
#include "game/layers/main_menu.h"
#include "raylib.h"
#include <array>
#include <format>
#include <iostream>
#include <stdexcept>
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

bool GameLayer::CanMoveInDirection(Entity* entity,
                                   const Vector2Ex<float>& position,
                                   const ui::Direction& direction) const
{
    using namespace ui;
    using enum AnchorPoint;
    using enum Direction;
    const Vector2Ex<float> pacmanDimensions = entity->GetDimensions();

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

bool GameLayer::TryApplyQueuedDirection(Entity* entity,
                                        Vector2Ex<float>& currentPosition,
                                        ui::Direction& currentDirection)
{
    ui::Direction queuedDir = entity->GetQueuedDirection();

    if (queuedDir == currentDirection)
        return false;

    const Vector2Ex<float> queuedVec = Vector2Ex<float>::GetDirectionVector(queuedDir);
    const Vector2Ex<float> peekPosition = currentPosition + queuedVec;

    if (CanMoveInDirection(entity, peekPosition, queuedDir))
    {
        entity->ApplyQueuedDirection();
        currentDirection = queuedDir;
        return true;
    }

    return false;
}

void GameLayer::RenderNodes() const
{
    const auto& nodes = m_board.GetNodeMap();
    for (const auto& [index, node] : nodes)
    {
        DrawCircleV(node->GetPosition(), 5, RED);

        const Arc& rightArc = node->GetRightArc();
        if (rightArc.GetEndNode() != nullptr)
        {
            DrawLineV(rightArc.GetStartNode()->GetPosition(), rightArc.GetEndNode()->GetPosition(), BLUE);
        }

        const Arc& downArc = node->GetDownArc();
        if (downArc.GetEndNode() != nullptr)
        {
            DrawLineV(downArc.GetStartNode()->GetPosition(), downArc.GetEndNode()->GetPosition(), BLUE);
        }
    }
}

GameLayer::GameLayer() :
    m_board(),
    m_player(game::GameApplication::Get().GetProfile(),
             Pacman(m_board.GetPlayerSpawnPoint(), m_board.GetTileDimensions(), 400)),
    m_blinky(m_board.GetSpeedyGhostSpawnPoint(),
             Vector2Ex<float>(350, 350),
             m_board.GetTileDimensions(),
             ui::Direction::RIGHT)
{
}

GameLayer::GameLayer(std::string_view boardPath) :
    m_board(boardPath),
    m_player(game::GameApplication::Get().GetProfile(),
             Pacman(m_board.GetPlayerSpawnPoint(), m_board.GetTileDimensions(), 400)),
    m_blinky(m_board.GetSpeedyGhostSpawnPoint(),
             Vector2Ex<float>(350, 350),
             m_board.GetTileDimensions(),
             ui::Direction::RIGHT)
{
}

GameLayer::~GameLayer()
{
    m_board.SaveHighscoresToFile();
}

void GameLayer::HandleKeyPresses()
{
    using enum ui::Direction;
    const auto& inputManager = game::GameApplication::GetInputManager();

    if (inputManager.IsAction("move_up", engine::InputState::PRESSED))
        m_player.GetPacman().SetQueuedDirection(UP);

    if (inputManager.IsAction("move_down", engine::InputState::PRESSED))
        m_player.GetPacman().SetQueuedDirection(DOWN);

    if (inputManager.IsAction("move_left", engine::InputState::PRESSED))
        m_player.GetPacman().SetQueuedDirection(LEFT);

    if (inputManager.IsAction("move_right", engine::InputState::PRESSED))
        m_player.GetPacman().SetQueuedDirection(RIGHT);

    if (IsKeyPressed(KEY_F1))
        m_board.SaveToFile();

    if (inputManager.IsAction("pause", engine::InputState::PRESSED))
    {
        SuspendUpdate();
        Push(std::make_unique<GameOptionsMenuLayer>());
    }
}

void GameLayer::HandleCollisions(Entity* entity, const float& deltaTime, const bool collectPellets)
{
    using namespace ui;

    Vector2Ex<float> currentPosition = entity->GetPositionAtAnchor();
    Direction currentDirection = entity->GetDirection();

    // Collect pellet at starting position
    CollectPelletAtPosition(currentPosition);

    // Calculate desired movement
    const Vector2Ex<float> targetPosition = entity->GetNextPosition(currentDirection, deltaTime);
    const Vector2Ex<float> movementDelta = targetPosition - currentPosition;

    // Number of intermediate steps to check
    float totalDistance = (currentDirection == Direction::UP || currentDirection == Direction::DOWN)
                              ? std::abs(movementDelta.y)
                              : std::abs(movementDelta.x);

    // Try to apply queued direction at start if stationary or at current position
    float remainingDistance = totalDistance;
    if (TryApplyQueuedDirection(entity, currentPosition, currentDirection))
    {
        // Direction changed, update current direction for the pacman
        currentDirection = entity->GetDirection();
    }

    if (totalDistance <= 0)
    {
        entity->SetPosition(currentPosition);
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
        if (TryApplyQueuedDirection(entity, intermediatePosition, currentDirection))
        {
            if (collectPellets)
                CollectPelletAtPosition(intermediatePosition);

            // Direction changed, update position and direction, then continue in new direction
            lastValidPosition = intermediatePosition;
            currentPosition = intermediatePosition;
            currentDirection = entity->GetDirection();
            continue;
        }

        // Check collision
        if (!CanMoveInDirection(entity, intermediatePosition, currentDirection))
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

    entity->SetPosition(lastValidPosition);
}

void GameLayer::UpdateHighscores()
{
    std::string_view boardName = m_board.GetName();

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
    HandleCollisions(&m_player.GetPacman(), ts, true);

    m_blinky.UpdateQueuedDirection(m_board, m_player.GetPacman().GetPositionAtAnchor());
    HandleCollisions(&m_blinky, ts, false);
}

void GameLayer::OnRender()
{
    m_board.Render();
    m_player.GetPacman().Render();
    m_blinky.Render();
    RenderScores();
    RenderNodes();
}

void GameLayer::RenderScores() const
{
    const int currentPoints = m_player.GetPoints();
    std::string_view boardName = m_board.GetName();
    const auto& highscores = game::GameApplication::Get().GetProfile()->GetPersonalHighscores();

    int highscore = 0;
    auto it = highscores.find(boardName);
    if (it != highscores.end())
    {
        highscore = it->second;
    }

    const std::string currentPointsStr = std::format("Score: {}", currentPoints);
    const std::string highscoreStr = std::format("Highscore: {}", highscore);

    DrawText(highscoreStr.c_str(), 10, 10, 20, BLACK);
    DrawText(currentPointsStr.c_str(), 10, 40, 20, BLACK);
}
