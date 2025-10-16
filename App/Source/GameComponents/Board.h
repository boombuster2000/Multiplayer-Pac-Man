#pragma once
#include "Tile.h"
#include <array>
#include "raylib.h"
#include "UIComponents/Grid.h"
#include "DataTypes/Vector2Ex.h"

class Board
{
private:
    Vector2Ex<int> m_tileDimensions;
    Vector2Ex<int> m_boardDimensions;
    UIComponents::Grid<Tile> m_grid;

public:
    Board();

    void SetTile(Vector2Ex<int> position, Tile::Type type);
    Vector2Ex<int> ConvertPositionToIndex(Vector2Ex<int> position);

    void Render(Vector2Ex<int> offset = {0, 0}) const;

private:
    void AddBoundaries();
};