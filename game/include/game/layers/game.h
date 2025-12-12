#pragma once
#include "engine/core/layer.h"
#include "engine/core/vector2ex.h"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/components/client.h"
#include "game/components/entity.h"
#include "game/components/ghost.h"
#include "game/components/ghost_blinky.h"
#include "game/components/ghost_clyde.h"
#include "game/components/ghost_inky.h"
#include "game/components/ghost_pinky.h"
#include "game/components/pacman.h"

#include <array>

class GameLayer : public engine::Layer
{
  private:
    Board m_board;

    std::vector<Client> m_clients;
    ui::Direction m_queuedDirection;
    Blinky m_blinky;
    Pinky m_pinky;
    Inky m_inky;
    Clyde m_clyde;
    std::array<Ghost*, 4> m_ghosts;

    float m_timePassedSinceLastSave = 0.0f;
    float m_timePassedSinceStart = 0.0f;
    float m_ghostModeTimer = 0.0f;
    float m_frightenedModeTimer = 0.0f;
    Ghost::State m_mainGhostMode = Ghost::State::CHASE; // Alternates from CHASE and SCATTER
    bool m_isGameOver = false;
    bool m_frightenedStateDebounce = false;
    bool m_isFrightenedModeEnabled = false;
    bool m_shouldResetDidJustDies = false;

  private:
    static bool IsPacmanTouchingPellet(const Pellet& pellet,
                                const Vector2Ex<float>& pacmanDimensions,
                                const Vector2Ex<float>& pacmanPosition);

    bool TryCollectPellet(Player& player,
                          const Vector2Ex<float>& pacmanPosition,
                          const Vector2Ex<float>& pacmanDimensions,
                          Pellet& pellet);

    bool TryCollectPellet(Player& player,
                          const Vector2Ex<float>& pacmanPosition,
                          const Vector2Ex<float>& pacmanDimensions,
                          const Vector2Ex<float>& tilePosition);

    void ProcessPelletCollection(Client& client, Vector2Ex<float> posBefore, Vector2Ex<float> posAfter);

    bool CanMoveInDirection(const Entity* entity,
                            const Vector2Ex<float>& position,
                            const ui::Direction& direction) const;
    bool TryApplyQueuedDirection(Entity* entity,
                                 const Vector2Ex<float>& currentPosition,
                                 ui::Direction& currentDirection) const;
    void RenderScores() const;
    void RenderNodes() const;
    void RenderLives() const;
    void SetPacmansSpawnPositions();

    Pacman& GetClosestAlivePacmanWithNodes(const Vector2Ex<float>& referencePoint) const;

    static bool IsPacmanTouchingGhost(const Pacman& pacman, const Ghost& ghost);
    void ProcessGhostCollisions();
    static void HandlePacmanDeath(Pacman& pacman, Ghost& ghost);
    static void HandleGhostDeath(Player& pacman, Ghost& ghost);
    int GetCurrentAlivePacmanCount() const;
    int GetPacmanWithLivesCount() const;

    void RespawnGhost(Ghost* ghost) const;
    void ProcessDeadGhost(Ghost* ghost) const;
    void HandleFrightenedGhostMovementDecision(Ghost* ghost) const;
    void Chase(Ghost* ghost) const;
    void Scatter(Ghost* ghost) const;

    void UpdateTimers(float ts);
    void SaveHighscoresToBoard();
    void UpdateGhostModes();
    void ProcessPacmans(float ts);
    void ProcessGhosts(float ts);

  public:
    explicit GameLayer(const std::vector<Client>& clients);
    GameLayer(const std::vector<Client>& clients, Board board);
    ~GameLayer() override;

    void HandleKeyPresses();
    void ProcessMovementSteps(Entity* entity, const float& deltaTime);
    void UpdateHighscores();

    void OnUpdate(float ts) final;
    void OnRender() final;
};
