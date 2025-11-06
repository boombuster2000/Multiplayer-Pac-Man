#pragma once
#include "Tile.h"
#include <array>
#include "raylib.h"
#include "UIComponents/Grid.h"
#include "DataTypes/Vector2Ex.h"

class Board : public UIComponents::Grid<Tile>
{
private:
    Vector2Ex<float> m_tileDimensions;
    Vector2Ex<float> m_boardDimensions;

public:
    Board();

    void SetTileType(const Vector2Ex<int> &index, const Tile::Type &type);
    Tile &GetTile(const Vector2Ex<int> &index);
    const Tile &GetTile(const Vector2Ex<int> &index) const;

private:
    void AddBoundaries();
};