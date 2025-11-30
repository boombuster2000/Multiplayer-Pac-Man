#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/grid.h"
#include "game/components/tile.h"
#include "game/utils/highscore_utils.h"
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

  private:
    void addBoundaries();

  public:
    Board();
    explicit Board(std::string_view filename);

    const std::string& GetName() const;

    Vector2Ex<float> GetPlayerSpawnPoint() const;

    Vector2Ex<float> GetSpeedyGhostSpawnPoint() const;

    void SetTileType(const Vector2Ex<int>& index, const Tile::Type& type);

    void SaveToFile() const;

    HighscoreMap GetHighscores() const;
    void SetHighscore(std::string_view profileName, int score);
    void SaveHighscoresToFile() const;

    static Board LoadFromFile(const std::string& filename);
    static Board LoadFromFile(const std::filesystem::path& filepath);
};