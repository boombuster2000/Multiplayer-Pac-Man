#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/grid.h"
#include "game/components/tile.h"
#include "raylib.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Board : public ui::Grid<Tile>
{
    friend void to_json(json& j, const Board& board);
    friend void from_json(const json& j, Board& board);

  private:
    std::string m_name;

  private:
    void addBoundaries();

  public:
    Board();
    Board(const std::string& filename);

    std::string GetName() const;

    Vector2Ex<float> GetPlayerSpawnPoint() const;

    void SetTileType(const Vector2Ex<int>& index, const Tile::Type& type);

    void SaveToFile() const;

    static Board LoadFromFile(const std::string& filename);
};