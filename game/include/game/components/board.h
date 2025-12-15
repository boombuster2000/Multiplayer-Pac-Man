#pragma once
#include "engine/core/vector2ex.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/grid.h"
#include "game/components/ghost.h"
#include "game/components/ghost_blinky.h"
#include "game/components/ghost_clyde.h"
#include "game/components/ghost_inky.h"
#include "game/components/ghost_pinky.h"
#include "game/components/tile.h"
#include "game/utils/highscore_utils.h"
#include "node_system.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using game::highscore_utils::HighscoreMap;
using NodeDistanceTable = std::unordered_map<Node*, std::unordered_map<Node*, float>>;
using NodeRouteTable = std::unordered_map<Node*, std::unordered_map<Node*, Node*>>;
using nlohmann::json;

class Board : public ui::Grid<Tile>
{
    friend void to_json(json& j, const Board& board);
    friend void from_json(const json& j, Board& board);

  private:
    std::string m_name;
    HighscoreMap m_highScores;
    std::unordered_map<Vector2Ex<size_t>, Node*> m_nodeMap;
    std::vector<Node*> m_nodes;

    NodeDistanceTable m_distanceTable;
    NodeRouteTable m_routeTable;

    Vector2Ex<float> m_pacmanSpeed{400, 400};
    Vector2Ex<float> m_ghostSpeed{300, 300};
    float m_speedIncreaseIncrement = 25;
    float m_ghostReleaseTimeDecrease = 1;

    Vector2Ex<size_t> m_blinkySpawnPointIndex{8, 8};
    Vector2Ex<size_t> m_blinkyGuardPointIndex{4, 1};
    float m_blinkyReleaseTime = 0;

    Vector2Ex<size_t> m_pinkySpawnPointIndex{7, 8};
    Vector2Ex<size_t> m_pinkyGuardPointIndex{10, 1};
    float m_pinkyReleaseTime = 5;

    Vector2Ex<size_t> m_inkySpawnPointIndex{6, 8};
    Vector2Ex<size_t> m_inkyGuardPointIndex{4, 12};
    float m_inkyReleaseTime = 10;

    Vector2Ex<size_t> m_clydeSpawnPointIndex{5, 8};
    Vector2Ex<size_t> m_clydeGuardPointIndex{10, 12};
    float m_clydeReleaseTime = 15;

    Vector2Ex<size_t> m_player1SpawnPointIndex{1, 1};
    Vector2Ex<size_t> m_player2SpawnPointIndex{12, 12};
    Vector2Ex<size_t> m_player3SpawnPointIndex{3, 10};
    Vector2Ex<size_t> m_player4SpawnPointIndex{10, 3};

    float m_frightenedTimeDuration = 5;
    float m_ghostModeTime = 5;

  private:
    void AddBoundaries();

    void AddArcsToNode(Node* node, const Vector2Ex<size_t>& index) const;
    void CreateNodes();
    void CreateDistanceTable();
    void CreateRouteTable();
    void Floyds();

    bool IsTileJunction(const Vector2Ex<size_t>& index) const;

    Vector2Ex<size_t> GetIndexOfNextJunction(const Vector2Ex<size_t>& startIndex, const ui::Direction& direction) const;

    void CreateNodesAndArcs();

  public:
    Board();
    explicit Board(std::string_view filename);
    const std::string& GetName() const;

    const std::unordered_map<Vector2Ex<size_t>, Node*>& GetNodeMap() const;

    std::vector<Node*> GetNodes() const;

    Node* GetClosestNode(const Vector2Ex<float> position) const;
    static bool IsAtNode(const Vector2Ex<float>& pos, const Vector2Ex<float>& nodePos);

    const NodeRouteTable& GetRouteTable() const;
    const NodeDistanceTable& GetDistanceTable() const;

    float GetDirectedDistanceThroughNodes(const Vector2Ex<float>& pos,
                                          const ui::Direction& direction,
                                          const Vector2Ex<float>& target) const;

    Vector2Ex<float> GetPlayerSpawnPoint(const int player) const;
    Vector2Ex<float> GetGhostSpawnPoint(const Ghost::Type ghostType) const;
    std::pair<Vector2Ex<float>, Vector2Ex<float>> GetGhostSpawnRegion() const;
    static bool IsInRegion(const std::pair<Vector2Ex<float>, Vector2Ex<float>>& region,
                           const Vector2Ex<float>& position);
    bool HasLineOfSight(const Vector2Ex<float>& pos1, const Vector2Ex<float>& pos2) const;

    void SetTileType(const Vector2Ex<size_t>& index, const Tile::Type& type);

    Vector2Ex<float> GetPacmanSpeed() const;
    Vector2Ex<float> GetGhostSpeed() const;
    Vector2Ex<float> GetSpeedIncrease() const;
    float GetGhostReleaseTimeDecrease() const;
    float GetFrightenedTime() const;
    float GetGhostModeTime() const;

    Blinky GetBlinky() const;

    Pinky GetPinky() const;

    Inky GetInky() const;

    Clyde GetClyde() const;

    bool AreAllPelletsEaten() const;
    void ResetPellets();

    void SaveToFile() const;

    HighscoreMap GetHighscores() const;
    void SetHighscore(std::string_view profileName, int score);
    void SaveHighscoresToFile() const;

    static Board LoadFromFile(const std::string& filename);
    static Board LoadFromFile(const std::filesystem::path& filepath);
};