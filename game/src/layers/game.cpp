#include "game/layers/game.h"
#include "engine/core/input_manager.h"
#include "game/game_application.h"
#include "game/layers/game_options_menu.h"
#include "game/layers/main_menu.h"
#include "raylib.h"
#include <array>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

bool GameLayer::IsPacmanTouchingPellet(const Pellet& pellet,
                                       const Vector2Ex<float>& pacmanDimensions,
                                       const Vector2Ex<float>& pacmanPosition) const
{
    const Vector2Ex<float> pelletPosition = pellet.GetPositionAtAnchor();
    const Vector2Ex<float> pelletDimensions = pellet.GetDimensions();
    const Rectangle pacmanRec = {pacmanPosition.x, pacmanPosition.y, pacmanDimensions.x, pacmanDimensions.y};
    const Rectangle pelletRec = {pelletPosition.x, pelletPosition.y, pelletDimensions.x, pelletDimensions.y};

    return CheckCollisionRecs(pacmanRec, pelletRec);
}

bool GameLayer::TryCollectPellet(Player& player,
                                 const Vector2Ex<float>& pacmanPosition,
                                 const Vector2Ex<float>& pacmanDimensions,
                                 Pellet& pellet)
{
    if (!IsPacmanTouchingPellet(pellet, pacmanDimensions, pacmanPosition))
        return false;

    int pointsGained = pellet.GetValue();
    player.AddPoints(pointsGained);
    UpdateHighscores();
    pellet.SetType(Pellet::Type::NONE);

    return pointsGained > 0;
}

bool GameLayer::TryCollectPellet(Player& player,
                                 const Vector2Ex<float>& pacmanPosition,
                                 const Vector2Ex<float>& pacmanDimensions,
                                 const Vector2Ex<float>& tilePosition)
{
    Tile& tile = m_board.GetTileFromPosition(tilePosition);
    Pellet& pellet = tile.GetPellet();

    return TryCollectPellet(player, pacmanPosition, pacmanDimensions, pellet);
}

void GameLayer::ProcessPelletCollection(Client& client,
                                        const Vector2Ex<float> posBefore,
                                        const Vector2Ex<float> posAfter)
{

    const Vector2Ex<float>& pacmanPosition = client.pacman.GetPositionAtAnchor();
    const Vector2Ex<float>& pacmanDimensions = client.pacman.GetDimensions();
    TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, posBefore);
    TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, posAfter);

    // Pacman at most have moved across a tile boundary, and pellets on those have already been checked for collection
    if ((posBefore - posAfter).GetLength() < 1)
        return;

    const Vector2Ex<size_t> posBeforeIndex = m_board.GetRelativeIndexFromPosition(posBefore);
    const Vector2Ex<size_t> posAfterIndex = m_board.GetRelativeIndexFromPosition(posAfter);
    const Vector2Ex<int> indexSteps = Vector2Ex<int>(posAfterIndex) - Vector2Ex<int>(posBeforeIndex);
    const Vector2Ex<int> indexStep = indexSteps.GetUnitVector();

    // I don't want to check tile at posBefore and posAfter as pacman may have passed the pellet at the centre of tile.
    // They will be checked seperatley
    for (int i = 1; i < indexStep.GetLength(); i++)
    {
        Tile& tile = m_board.GetTile(posBeforeIndex + indexStep);
        Pellet& pellet = tile.GetPellet();

        TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, pellet);
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

Blinky GameLayer::ConstructBlinky() const
{
    return Blinky(m_board.GetSpeedyGhostSpawnPoint(),
                  Vector2Ex<float>(350, 350),
                  m_board.GetTileDimensions(),
                  ui::Direction::RIGHT);
}

GameLayer::GameLayer(const std::vector<Client>& clients) :
    m_board(),
    m_blinky(ConstructBlinky()),
    m_clients(clients)
{
}

GameLayer::GameLayer(const std::vector<Client>& clients, std::string_view boardPath) :
    m_board(boardPath),
    m_blinky(ConstructBlinky()),
    m_clients(clients)

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

    for (auto& client : m_clients)
    {
        Pacman& pacman = client.pacman;
        const PlayerInput* input = client.input;

        if (input->IsUpPressed())
            pacman.SetQueuedDirection(UP);

        if (input->IsDownPressed())
            pacman.SetQueuedDirection(DOWN);

        if (input->IsLeftPressed())
            pacman.SetQueuedDirection(LEFT);

        if (input->IsRightPressed())
            pacman.SetQueuedDirection(RIGHT);
    }

    if (IsKeyPressed(KEY_F1))
        m_board.SaveToFile();

    if (inputManager.IsAction("pause", engine::InputState::PRESSED))
    {
        SuspendUpdate();
        Push(std::make_unique<GameOptionsMenuLayer>());
    }
}

void GameLayer::ProcessMovementSteps(Entity* entity, const float& deltaTime)
{
    // TODO: Refactor to run less iteractions
    using namespace ui;

    Vector2Ex<float> currentPosition = entity->GetPositionAtAnchor();
    Direction currentDirection = entity->GetDirection();

    // // Collect pellet at starting
    // if (entity->GetEntityType() == EntityType::PACMAN)
    //     ProcessPelletCollection(currentPosition);

    // Calculate desired movement
    const Vector2Ex<float> targetPosition = entity->GetNextPosition(currentDirection, deltaTime);
    const Vector2Ex<float> movementDelta = targetPosition - currentPosition;

    // Number of intermediate steps to check
    float totalDistance = movementDelta.y + movementDelta.x; // One of these will always be 0.

    // Try to apply queued direction at start if stationary or at current position
    float remainingDistance = totalDistance;
    if (TryApplyQueuedDirection(entity, currentPosition, currentDirection))
        // Direction changed, update current direction for the pacman
        currentDirection = entity->GetDirection();

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
        // if remainingDistnace is less that 1, remainingDistance will be used as the step so loop will run once
        float stepSize = std::min(remainingDistance, 1.0f);

        // Calculate the intermediate position using current direction
        Vector2Ex<float> directionVector = Vector2Ex<float>::GetDirectionVector(currentDirection);
        Vector2Ex<float> intermediatePosition = currentPosition + (directionVector * stepSize);

        // Try to apply queued direction at this position
        if (TryApplyQueuedDirection(entity, intermediatePosition, currentDirection))
        {
            // if (entity->GetEntityType() == EntityType::PACMAN)
            //     ProcessPelletCollection(intermediatePosition);

            // Direction changed, update position and direction, then continue in new direction
            lastValidPosition = intermediatePosition;
            currentPosition = intermediatePosition;
            currentDirection = entity->GetDirection();
            continue;
        }

        // Check collision
        if (!CanMoveInDirection(entity, intermediatePosition, currentDirection))
            // Hit a wall, stop at last valid position
            break;

        // // position is valid, collect pellets and continue
        // if (entity->GetEntityType() == EntityType::PACMAN)
        //     ProcessPelletCollection(intermediatePosition);

        lastValidPosition = intermediatePosition;
        currentPosition = intermediatePosition;
        remainingDistance -= stepSize;
    }

    entity->SetPosition(lastValidPosition);
}

void GameLayer::UpdateHighscores()
{
    std::string_view boardName = m_board.GetName();

    for (auto& client : m_clients)
    {
        std::shared_ptr<Profile> profile = client.profile;
        const int points = client.player.GetPoints();

        client.profile->UpdateHighScore(boardName, points);
        m_board.SetHighscore(profile->GetUsername(), points);
    }
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

    for (auto& client : m_clients)
    {
        ProcessMovementSteps(&client.pacman, ts);
        ProcessPelletCollection(client, client.pacman.GetLastPosition(), client.pacman.GetPositionAtAnchor());
    }

    m_blinky.UpdateQueuedDirection(m_board, m_clients[0].pacman.GetPositionAtAnchor());
    ProcessMovementSteps(&m_blinky, ts);
}

void GameLayer::OnRender()
{
    m_board.Render();

    for (auto& client : m_clients)
        client.pacman.Render();

    m_blinky.Render();
    RenderScores();
    RenderNodes();
}

void GameLayer::RenderScores() const
{
    const int currentPoints = m_clients[0].player.GetPoints();
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
