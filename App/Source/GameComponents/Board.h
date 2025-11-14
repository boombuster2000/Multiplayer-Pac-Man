#pragma once
#include "DataTypes/Vector2Ex.h"
#include "Tile.h"
#include "UIComponents/Grid.h"
#include "raylib.h"

class Board : public UIComponents::Grid<Tile>
{

  private:
    void AddBoundaries();

  public:
    Board();

    void SetTileType(const Vector2Ex<int> &index, const Tile::Type &type);

    void SaveToFile(const std::string &filename) const;

    static Board LoadFromFile(const std::string &filename);
};