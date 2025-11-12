#pragma once
#include "Tile.h"
#include <array>
#include "raylib.h"
#include "UIComponents/Grid.h"
#include "DataTypes/Vector2Ex.h"

class Board : public UIComponents::Grid<Tile>
{
public:
    Board();

    void SetTileType(const Vector2Ex<int> &index, const Tile::Type &type);
private:
    void AddBoundaries();
};