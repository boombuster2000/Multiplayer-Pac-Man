#include "game/layers/game.h"
#include "engine/core/input_manager.h"
#include "game/game_application.h"
#include "game/layers/game_options_menu.h"
#include "game/layers/main_menu.h"
#include "raylib.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <format>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

bool GameLayer::IsPacmanTouchingPellet(const Pellet& pellet,
                                       const Vector2Ex<float>& pacmanDimensions,
                                       const Vector2Ex<float>& pacmanPosition)
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

    const int pointsGained = pellet.GetValue();
    player.AddPoints(pointsGained);

    if (const Pellet::Type pelletType = pellet.GetType(); pelletType == Pellet::Type::SUPER)
        m_isFrightenedModeEnabled = true;

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

    const Vector2Ex<size_t> posBeforeIndex = m_board.GetRelativeIndexFromPosition(posBefore);
    const Vector2Ex<size_t> posAfterIndex = m_board.GetRelativeIndexFromPosition(posAfter);

    if (posBeforeIndex == posAfterIndex)
        return;

    const Vector2Ex<int> indexDiff(static_cast<int>(posAfterIndex.x - posBeforeIndex.x), static_cast<int>(posAfterIndex.y - posBeforeIndex.y));

    // If there's a diagonal change in tile indices, it's a corner turn.
    if (indexDiff.x != 0 && indexDiff.y != 0)
    {
        // A turn means Pacman moved in an L-shape. The corner of the L is where a pellet might be missed.
        // There are two possible corners. We check both.
        const Vector2Ex<size_t> corner1Index(posAfterIndex.x, posBeforeIndex.y);
        Tile& corner1Tile = m_board.GetTile(corner1Index);
        TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, corner1Tile.GetPellet());

        const Vector2Ex<size_t> corner2Index(posBeforeIndex.x, posAfterIndex.y);
        Tile& corner2Tile = m_board.GetTile(corner2Index);
        TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, corner2Tile.GetPellet());
    }

    // Check for pellets skipped during fast straight movement
    if (indexDiff.y == 0 && std::abs(indexDiff.x) > 1)
    {
        const int step = (indexDiff.x > 0) ? 1 : -1;
        for (int i = step; std::abs(i) < std::abs(indexDiff.x); i += step)
        {
            Tile& tile = m_board.GetTile({posBeforeIndex.x + i, posBeforeIndex.y});
            TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, tile.GetPellet());
        }
    }
    else if (indexDiff.x == 0 && std::abs(indexDiff.y) > 1)
    {
        const int step = (indexDiff.y > 0) ? 1 : -1;
        for (int i = step; std::abs(i) < std::abs(indexDiff.y); i += step)
        {
            Tile& tile = m_board.GetTile({posBeforeIndex.x, posBeforeIndex.y + i});
            TryCollectPellet(client.player, pacmanPosition, pacmanDimensions, tile.GetPellet());
        }
    }
}

bool GameLayer::CanMoveInDirection(const Entity* entity,
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
        if (const Tile& tile = m_board.GetTileFromPosition(corner); tile.GetType() == Tile::Type::WALL)
            return false;
    }

    return true;
}

bool GameLayer::TryApplyQueuedDirection(Entity* entity,
                                        const Vector2Ex<float>& currentPosition,
                                        ui::Direction& currentDirection) const
{
    const ui::Direction queuedDir = entity->GetQueuedDirection();

    if (queuedDir == currentDirection)
        return false;

    const Vector2Ex<float> queuedVec = Vector2Ex<float>::GetDirectionVector(queuedDir);

    if (const Vector2Ex<float> peekPosition = currentPosition + queuedVec;
        CanMoveInDirection(entity, peekPosition, queuedDir))
    {
        entity->ApplyQueuedDirection();
        currentDirection = queuedDir;
        return true;
    }

    return false;
}

void GameLayer::RenderNodes() const
{
    for (const auto& nodes = m_board.GetNodeMap(); const auto& node : nodes | std::views::values)
    {
        DrawCircleV(node->GetPosition(), 5, RED);

        if (const Arc& rightArc = node->GetRightArc(); rightArc.GetEndNode() != nullptr)
        {
            DrawLineV(rightArc.GetStartNode()->GetPosition(), rightArc.GetEndNode()->GetPosition(), BLUE);
        }

        if (const Arc& downArc = node->GetDownArc(); downArc.GetEndNode() != nullptr)
        {
            DrawLineV(downArc.GetStartNode()->GetPosition(), downArc.GetEndNode()->GetPosition(), BLUE);
        }
    }
}
void GameLayer::RenderLives() const
{
    using enum ui::AnchorPoint;
    float yOffset = 10;
    constexpr float xOffset = 30;

    Vector2Ex<float> startPosition = m_board.GetPositionAtAnchor(TOP_RIGHT);
    startPosition.x += xOffset;

    for (const auto& client : m_clients)
    {
        constexpr float radius = 17;
        constexpr float ySpacing = 10;
        const Color pacmanColor = client.pacman.GetColor();
        std::shared_ptr<Texture2D> texture = client.pacman.GetTexture();
        const int lives = client.pacman.GetLives();

        startPosition.y += yOffset;

        Vector2Ex<float> position = startPosition;
        for (int i = 0; i < lives; i++)
        {
            constexpr float xSpacing = 3;
            const float scale = radius / static_cast<float>(texture->width);

            DrawTextureEx(*texture, position,0, scale, pacmanColor);
            position.x += radius + xSpacing;
        }

        yOffset += radius + ySpacing;
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

Pacman& GameLayer::GetClosestAlivePacmanWithNodes(const Vector2Ex<float>& referencePoint) const
{
    if (m_clients.empty())
        throw std::runtime_error("GetClosestAlivePacmanWithNodes called with no clients");


    const NodeDistanceTable& distanceTable = m_board.GetDistanceTable();
    Node* referenceNode = m_board.GetClosestNode(referencePoint);

    const Pacman* closestPacman = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (const auto& client : m_clients)
    {
        const Pacman& currentPacman = client.pacman;

        if (currentPacman.IsDead())
            continue;

        Node* pacmanNode = m_board.GetClosestNode(currentPacman.GetPositionAtAnchor());

        // Check if a route exists between the nodes
        if (auto refIt = distanceTable.find(referenceNode); refIt != distanceTable.end())
        {
            if (auto pacmanIt = refIt->second.find(pacmanNode); pacmanIt != refIt->second.end())
            {
                // Distance between nodes from the pre-calculated table
                const float nodeDistance = pacmanIt->second;

                // Distance from reference point to its closest node
                const float distToRefNode = (referencePoint - referenceNode->GetPosition()).GetLength();

                // Distance from pacman to its closest node
                const float distFromPacmanNode =
                    (currentPacman.GetPositionAtAnchor() - pacmanNode->GetPosition()).GetLength();

                if (const float totalDistance = distToRefNode + nodeDistance + distFromPacmanNode;
                    totalDistance < minDistance)
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

GameLayer::GameLayer(const std::vector<Client>& clients) :
    m_board(),
    m_clients(clients),
    m_blinky(m_board.GetBlinky()),
    m_pinky(m_board.GetPinky()),
    m_inky(m_board.GetInky()),
    m_clyde(m_board.GetClyde()),
    m_ghosts{&m_blinky, &m_pinky, &m_inky, &m_clyde}
{
    SetPacmansSpawnPositions();
    srand(time(nullptr));
}

GameLayer::GameLayer(const std::vector<Client>& clients, Board board) :
    m_board(std::move(board)),
    m_clients(clients),
    m_blinky(m_board.GetBlinky()),
    m_pinky(m_board.GetPinky()),
    m_inky(m_board.GetInky()),
    m_clyde(m_board.GetClyde()),
    m_ghosts{&m_blinky, &m_pinky, &m_inky, &m_clyde}
{
    SetPacmansSpawnPositions();
    srand(time(nullptr));
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

bool GameLayer::IsPacmanTouchingGhost(const Pacman& pacman, const Ghost& ghost) const
{
    const Vector2Ex<float> pacmanPosition = pacman.GetPositionAtAnchor();
    const Vector2Ex<float> pacmanDimensions = pacman.GetDimensions();
    const Vector2Ex<float> ghostPosition = ghost.GetPositionAtAnchor();
    const Vector2Ex<float> ghostDimensions = ghost.GetDimensions();

    const Rectangle pacmanRec = {pacmanPosition.x, pacmanPosition.y, pacmanDimensions.x, pacmanDimensions.y};
    const Rectangle ghostRec = {ghostPosition.x, ghostPosition.y, ghostDimensions.x, ghostDimensions.y};

    return CheckCollisionRecs(pacmanRec, ghostRec);
}

void GameLayer::HandlePacmanDeath(Pacman& pacman, Ghost& ghost)
{
    pacman.RemoveLife();
    pacman.SetDead(true);
    pacman.SetRespawnTimer(3.0f); // Pac-man is "dead" for 3 seconds.

    // Making pacman slightly transparent
    Color newColor = pacman.GetColor();
    newColor.a = 128;
    pacman.SetColor(newColor);

    if (pacman.GetLives() > 0)
        pacman.SetPosition(pacman.GetSpawnPosition());
}

void GameLayer::HandleGhostDeath(Player& player, Ghost& ghost)
{
    Color newColor = ghost.GetColor();
    newColor.a = 128;
    ghost.SetColor(newColor);

    player.AddPoints(200); // points gained by kill ghost
    ghost.SetState(Ghost::State::DEAD);
}

void GameLayer::ProcessGhostCollisions()
{
    for (auto& client : m_clients)
    {
        if (client.pacman.IsDead()) // Already dead, no need to check collisions
            continue;

        for (const auto& ghost : m_ghosts)
        {
            if (ghost->GetState() == Ghost::State::DEAD)
                continue;

            if (IsPacmanTouchingGhost(client.pacman, *ghost))
            {
                if (!m_isFrightenedModeEnabled)
                {
                    HandlePacmanDeath(client.pacman, *ghost);
                    break; // One collision is enough to kill, move to next client
                }
                else
                {
                    HandleGhostDeath(client.player, *ghost);
                }

            }
        }
    }
}

int GameLayer::GetCurrentAlivePacmanCount() const
{
    int aliveCount = 0;
    for (const auto& client : m_clients)
    {
        if (!client.pacman.IsDead())
            aliveCount++;
    }

    return aliveCount;
}

int GameLayer::GetPacmanWithLivesCount() const
{
    int pacmanWithLivesCount = 0;
    for (const auto& client : m_clients)
    {
        if (client.pacman.GetLives() > 0)
            pacmanWithLivesCount++;
    }

    return pacmanWithLivesCount;
}

void GameLayer::ProcessMovementSteps(Entity* entity, const float& deltaTime)
{
    // TODO: Refactor to run less interactions
    using namespace ui;

    Vector2Ex<float> currentPosition = entity->GetPositionAtAnchor();
    Direction currentDirection = entity->GetDirection();

    // Calculate desired movement
    const Vector2Ex<float> targetPosition = entity->GetNextPosition(currentDirection, deltaTime);
    const Vector2Ex<float> movementDelta = targetPosition - currentPosition;

    // Total distance to move this frame
    const float totalDistance = movementDelta.GetLength();
    float remainingDistance = totalDistance;

    // Try to apply queued direction at start if stationary or at current position
    TryApplyQueuedDirection(entity, currentPosition, currentDirection);

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
            // Snap position to node to correct any drift. Node positions are at the top-left of tiles.
            // Direction changed, update position and direction, then continue in new direction
            lastValidPosition = intermediatePosition;
            currentPosition = intermediatePosition;
            continue;
        }

        // Check collision
        if (!CanMoveInDirection(entity, intermediatePosition, currentDirection))
        {
            // Hit a wall, stop at last valid position.
            // Snap the coordinate
            const Tile& currentTile = m_board.GetTileFromPosition(lastValidPosition);
            lastValidPosition = currentTile.GetPositionAtAnchor();
            entity->SetPosition(lastValidPosition + entity->GetObjectOrigin());
            break;
        }

        lastValidPosition = intermediatePosition;
        currentPosition = intermediatePosition;
        remainingDistance -= stepSize;
    }

    entity->SetPosition(lastValidPosition);
}

void GameLayer::UpdateHighscores()
{
    const std::string_view boardName = m_board.GetName();

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
    m_timePassedSinceStart += ts;
    m_ghostModeTimer += ts;

    if (m_ghostMode == Ghost::State::FRIGHTENED)
        m_ghostModeTimer += ts;


    if (m_timePassedSinceLastSave >= 10.0f)
    {
        m_board.SaveHighscoresToFile();
        m_timePassedSinceLastSave = 0.0f;
    }

    // Keeps track of what mode they should return to.
    if ( m_ghostModeTimer >= 15.0f)
    {
        m_ghostModeTimer = 0.0f;
        if (m_ghostMode == Ghost::State::SCATTER || m_ghostMode == Ghost::State::CHASE)
            m_ghostMode = (m_ghostMode == Ghost::State::SCATTER) ? Ghost::State::CHASE : Ghost::State::SCATTER;
    }

    if (m_isFrightenedModeEnabled)
        m_frightenedModeTimer += ts;

    // Return back to normal mode
    if (m_isFrightenedModeEnabled && m_frightenedModeTimer >= 10.0f)
    {
        m_frightenedModeTimer = 0.0f;
        m_frightenedStateDebounce = false;
        m_isFrightenedModeEnabled = false;
    }

    HandleKeyPresses();

    if (m_isGameOver)
    {
        // TODO: Render "Game Over" screen or transition to a game over state.
        return;
    }

    m_isGameOver = GetPacmanWithLivesCount() <= 0;

    // Process pacmans
    for (auto& client : m_clients)
    {
        if (client.pacman.IsDead())
        {
            client.pacman.UpdateRespawnTimer(ts);

            // Pacman comes to life again
            if (client.pacman.GetRespawnTimer() <= 0.f && client.pacman.GetLives() > 0)
            {
                client.pacman.SetDead(false);

                // Making solid color again.
                Color color = client.pacman.GetColor();
                color.a = 255;
                client.pacman.SetColor(color);
            }

        }

        // If permanently dead they can't move
        if (client.pacman.GetLives() > 0)
            ProcessMovementSteps(&client.pacman, ts);

        if (!client.pacman.IsDead())
            ProcessPelletCollection(client, client.pacman.GetLastPosition(), client.pacman.GetPositionAtAnchor());

    }

    ProcessGhostCollisions(); // Check for collisions after Pac-Mans have moved

    const int alivePacmanCount = GetCurrentAlivePacmanCount();

    // Process Ghosts
    for (const auto& ghost : m_ghosts)
    {
        // Make ghost travel back to spawn
        if (ghost->GetState() == Ghost::State::DEAD)
        {
            if (Board::IsInRegion(m_board.GetGhostSpawnRegion(), ghost->GetPositionAtAnchor()))
            {
                ghost->SetState(m_ghostMode);
                Color newColor = ghost->GetColor();
                newColor.a = 255;
                ghost->SetColor(newColor);
            }
            else
            {
                ghost->UpdateQueuedDirection(m_board, ghost->GetSpawnPosition());
            }

            ProcessMovementSteps(ghost, ts);
            continue;
        }

        // If it's time to release a spawning ghost, change its state to the current global mode.
        if (ghost->GetState() == Ghost::State::SPAWNING && ghost->GetReleaseTime() <= m_timePassedSinceStart)
        {
            if (m_isFrightenedModeEnabled)
                ghost->SetState(Ghost::State::FRIGHTENED);
            else
                ghost->SetState(m_ghostMode);
        }

        // Ghosts do nothing (including moving) while in the SPAWNING state.
        if (ghost->GetState() == Ghost::State::SPAWNING)
            continue;

        // Change ghosts to different states.
        if (m_isFrightenedModeEnabled)
        {
            ghost->SetSpeed({200,200});
            ghost->SetState(Ghost::State::FRIGHTENED);
        }
        else
        {
            ghost->SetSpeed({300,300});
            ghost->SetState(m_ghostMode);
        }

        if (m_isFrightenedModeEnabled)
        {
            // Should immediately go in opposite direction
            if (!m_frightenedStateDebounce)
            {
                ghost->SetQueuedDirection(ui::GetOppositeDirection(ghost->GetDirection()));
                m_frightenedStateDebounce = true;
            }

            // Can only change directions at nodes.
            if (Board::IsAtNode(ghost->GetPositionAtAnchor(), m_board.GetClosestNode(ghost->GetPositionAtAnchor())->GetPosition()))
            {
                // In frightened mode, ghosts move randomly.
                std::vector<ui::Direction> possibleDirections;
                constexpr std::array<ui::Direction, 4> allDirections = {
                    ui::Direction::UP, ui::Direction::DOWN, ui::Direction::LEFT, ui::Direction::RIGHT};

                const Vector2Ex<float> currentPos = ghost->GetPositionAtAnchor();

                for (const auto& direction : allDirections)
                {
                    if (const Vector2Ex<float> peekPosition = currentPos + Vector2Ex<float>::GetDirectionVector(direction);
                        CanMoveInDirection(ghost, peekPosition, direction))
                    {
                        possibleDirections.push_back(direction);
                    }
                }

                if (possibleDirections.size() > 1)
                {
                    const ui::Direction oppositeDirection = ui::GetOppositeDirection(ghost->GetDirection());
                    // Don't move in the opposite direction unless it's the only option
                    if (auto it = std::ranges::find(possibleDirections, oppositeDirection);
                        it != possibleDirections.end())
                    {
                        possibleDirections.erase(it);
                    }
                }

                if (!possibleDirections.empty())
                {
                    // Choose a random direction from the possible options
                    const size_t randomIndex = rand() % possibleDirections.size();
                    ghost->SetQueuedDirection(possibleDirections[randomIndex]);
                }
            }
        }

        else if (const Ghost::State state = ghost->GetState(); state == Ghost::State::CHASE)
        {
            // Ghosts only update their queued direction in and move if there's at least one alive Pacman
            if (alivePacmanCount > 0)
            {
                const Pacman& closestPacman = GetClosestAlivePacmanWithNodes(ghost->GetPositionAtAnchor());
                ghost->Update(m_board, closestPacman.GetPositionAtAnchor(), closestPacman.GetDirection());
            }
        }
        else if (state == Ghost::State::SCATTER)
        {
            // In scatter mode, ghosts target their designated corner (guard position).
            ghost->Update(m_board, ghost->GetGuardPosition(), ui::Direction::UP); // Pacman direction not relevant here
        }


        ProcessMovementSteps(ghost, ts);
    }

}

void GameLayer::OnRender()
{
    m_board.Render();

    for (const auto& client : m_clients)
    {
        const std::string_view username = client.profile->GetUsername();
        const Color playerColor = client.pacman.GetColor();
        client.pacman.Render();

        // Calculate text dimensions
        const char* usernameCStr = username.data();
        constexpr int fontSize = 20;
        const Vector2Ex<float> pacmanPosition = client.pacman.GetPositionAtAnchor();
        const Vector2Ex<float> pacmanDimensions = client.pacman.GetDimensions();

        const float textWidth = MeasureText(usernameCStr, fontSize);
        const float centerX = pacmanPosition.x + (pacmanDimensions.x / 2);
        const float textX = centerX - (textWidth / 2);
        const float textY = pacmanPosition.y - fontSize - 15; // Increased gap for triangle

        DrawText(usernameCStr, static_cast<int>(textX), static_cast<int>(textY), fontSize, playerColor);
    }



    for (const auto& ghost : m_ghosts)
        ghost->Render();

    RenderScores();
    RenderLives();
    // RenderNodes();
}

void GameLayer::RenderScores() const
{
    const std::string_view boardName = m_board.GetName();
    int yOffset = 10;

    for (const auto& client : m_clients)
    {
        constexpr int lineSpacing = 5;
        constexpr int playerSpacing = 20;
        constexpr int lineHeight = 20;

        std::string_view username = client.profile->GetUsername();
        const Color playerColor = client.pacman.GetColor();
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
        DrawText(usernameText.c_str(), 10, yOffset, lineHeight, playerColor); // Using DARKBLUE for username
        yOffset += lineHeight + lineSpacing;

        // Display Current Score in green
        const std::string currentPointsStr = std::format("Score: {}", currentPoints);
        DrawText(currentPointsStr.c_str(), 10, yOffset, lineHeight, playerColor); // Using GREEN for current score
        yOffset += lineHeight + lineSpacing;

        // Display Highscore in gold/yellow
        const std::string highscoreStr = std::format("Highscore: {}", highscore);
        DrawText(highscoreStr.c_str(), 10, yOffset, lineHeight, playerColor); // Using ORANGE for highscore
        yOffset += lineHeight + playerSpacing;                                // Add extra spacing for the next player
    }
}

