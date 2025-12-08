#include "game/layers/game.h"
#include "engine/core/input_manager.h"
#include "game/game_application.h"
#include "game/layers/game_options_menu.h"
#include "game/layers/main_menu.h"
#include "raylib.h"
#include <array>
#include <format>
#include <iostream>
#include <limits>
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

void GameLayer::SetPacmansSpawnPositions()
{
    int player = 1;
    for (auto& client : m_clients)
    {
        const Vector2Ex<float> position = m_board.GetPlayerSpawnPoint(player);
        client.pacman.SetSpawnPosition(position);
        client.pacman.SetPosition(position);
        player++;
    }
}

Pacman& GameLayer::GetClosestPacmanWithNodes(const Vector2Ex<float>& referencePoint) const
{
    if (m_clients.empty())
    {
        throw std::runtime_error("GetClosestPacmanWithNodes called with no clients");
    }

    const NodeDistanceTable& distanceTable = m_board.GetDistanceTable();
    Node* referenceNode = m_board.GetClosestNode(referencePoint);

    const Pacman* closestPacman = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (const auto& client : m_clients)
    {
        const Pacman& currentPacman = client.pacman;
        Node* pacmanNode = m_board.GetClosestNode(currentPacman.GetPositionAtAnchor());

        // Check if a route exists between the nodes
        auto refIt = distanceTable.find(referenceNode);
        if (refIt != distanceTable.end())
        {
            auto pacmanIt = refIt->second.find(pacmanNode);
            if (pacmanIt != refIt->second.end())
            {
                // Distance between nodes from the pre-calculated table
                float nodeDistance = pacmanIt->second;

                // Distance from reference point to its closest node
                float distToRefNode = (referencePoint - referenceNode->GetPosition()).GetLength();

                // Distance from pacman to its closest node
                float distFromPacmanNode =
                    (currentPacman.GetPositionAtAnchor() - pacmanNode->GetPosition()).GetLength();

                float totalDistance = distToRefNode + nodeDistance + distFromPacmanNode;

                if (totalDistance < minDistance)
                {
                    minDistance = totalDistance;
                    closestPacman = &currentPacman;
                }
            }
        }
    }

    if (closestPacman == nullptr)
    {
        // This can happen if no path is found to any pacman.
        // Fallback to the first pacman as a default.
        return const_cast<Pacman&>(m_clients[0].pacman);
    }

    return const_cast<Pacman&>(*closestPacman);
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
    SetPacmansSpawnPositions();
}

GameLayer::GameLayer(const std::vector<Client>& clients, std::string_view boardPath) :
    m_board(boardPath),
    m_blinky(ConstructBlinky()),
    m_clients(clients)

{
    SetPacmansSpawnPositions();
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

    // Total distance to move this frame
    float totalDistance = movementDelta.GetLength();

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

    Pacman& closestPacman = GetClosestPacmanWithNodes(m_blinky.GetPositionAtAnchor());
    m_blinky.UpdateQueuedDirection(m_board, closestPacman.GetPositionAtAnchor());
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
    std::string_view boardName = m_board.GetName();
    int yOffset = 10;
    const int lineHeight = 20;
    const int lineSpacing = 5;    // Spacing between lines for the same player's info
    const int playerSpacing = 20; // Extra spacing between different players

    for (const auto& client : m_clients)
    {
        std::string_view username = client.profile->GetUsername();
        const int currentPoints = client.player.GetPoints();
        const auto& personalHighscores = client.profile->GetPersonalHighscores();

        int highscore = 0;
        auto it = personalHighscores.find(boardName);
        if (it != personalHighscores.end())
        {
            highscore = it->second;
        }

        // Display Username in a distinct color
        const std::string usernameText = std::format("Username: {}", username);
        DrawText(usernameText.c_str(), 10, yOffset, lineHeight, DARKBLUE); // Using DARKBLUE for username
        yOffset += lineHeight + lineSpacing;

        // Display Current Score in green
        const std::string currentPointsStr = std::format("Score: {}", currentPoints);
        DrawText(currentPointsStr.c_str(), 10, yOffset, lineHeight, GREEN); // Using GREEN for current score
        yOffset += lineHeight + lineSpacing;

        // Display Highscore in gold/yellow
        const std::string highscoreStr = std::format("Highscore: {}", highscore);
        DrawText(highscoreStr.c_str(), 10, yOffset, lineHeight, ORANGE); // Using ORANGE for highscore
        yOffset += lineHeight + playerSpacing;                           // Add extra spacing for the next player
    }
}
