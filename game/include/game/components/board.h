#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/grid.h"
#include "game/components/tile.h"
#include "raylib.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

class Board : public ui::Grid<Tile>
{
    friend void to_json(json& j, const Board& board);
    friend void from_json(const json& j, Board& board);

  private:
    std::string m_name;
    std::unordered_map<std::string, int> m_highScores;

  private:
    void addBoundaries();
    void SortHighscores();

  public:
    Board();
    Board(const std::string& filename);

    std::string GetName() const;

    Vector2Ex<float> GetPlayerSpawnPoint() const;

    void SetTileType(const Vector2Ex<int>& index, const Tile::Type& type);

    void SaveToFile() const;

    std::unordered_map<std::string, int> GetHighscores() const;
    void SetHighscore(const std::string& profileName, int score);

    static Board LoadFromFile(const std::string& filename);
};