#pragma once
#include "engine/core/vector2ex.h"
#include "engine/core/vector2ex_hasher.h"
#include "engine/ui/grid.h"
#include "game/components/tile.h"
#include "game/utils/highscore_utils.h"
#include "node_system.h"
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using game::highscore_utils::HighscoreMap;
using nlohmann::json;

class Board : public ui::Grid<Tile>
{
    friend void to_json(json& j, const Board& board);
    friend void from_json(const json& j, Board& board);

  private:
    std::string m_name;
    HighscoreMap m_highScores;
    std::unordered_map<Vector2Ex<size_t>, Node*> m_nodes;

    std::unordered_map<Node*, std::unordered_map<Node*, float>> m_distanceMap;
    // std::unordered_map<Node*, std::unordered_map<Node*, Node*>> m_routeTable;

  private:
    void AddBoundaries();

    void AddArcsToNode(Node* node, const Vector2Ex<size_t>& index);
    void CreateNodes();
    void CreateDistanceMap();

    bool IsTileJunction(const Vector2Ex<size_t>& index) const;
    bool HasLineOfSight(const Vector2Ex<float>& pos1, const Vector2Ex<float>& pos2) const;

    Vector2Ex<size_t> GetIndexOfNextJunction(const Vector2Ex<size_t>& startIndex, const ui::Direction& direction) const;

    void CreateNodesAndArcs();

  public:
    Board();
    explicit Board(std::string_view filename);

    const std::string& GetName() const;

    const std::unordered_map<Vector2Ex<size_t>, Node*>& GetNodeMap() const;

    std::vector<Node*> GetNodes() const;

    Node* GetClosestNode(const Vector2Ex<float> position) const;

    Vector2Ex<float> GetPlayerSpawnPoint() const;

    Vector2Ex<float> GetSpeedyGhostSpawnPoint() const;

    void SetTileType(const Vector2Ex<size_t>& index, const Tile::Type& type);

    void SaveToFile() const;

    HighscoreMap GetHighscores() const;
    void SetHighscore(std::string_view profileName, int score);
    void SaveHighscoresToFile() const;

    static Board LoadFromFile(const std::string& filename);
    static Board LoadFromFile(const std::filesystem::path& filepath);
};