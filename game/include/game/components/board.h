#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/grid.h"
#include "game/components/tile.h"
#include "raylib.h"

class Board : public ui::Grid<Tile>
{

  private:
    void addBoundaries();

  public:
    Board();
    Board(const std::string& filename);

    Vector2Ex<float> GetPlayerSpawnPoint() const;
    void SetTileType(const Vector2Ex<int>& index, const Tile::Type& type);

    void SaveToFile(const std::string& filename) const;

    static Board LoadFromFile(const std::string& filename);
};