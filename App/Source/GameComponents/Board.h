#pragma once
#include "Tile.h"
#include <array>
#include "raylib.h"

class Board
{
    private:
    Vector2 m_tileDimensions;
    Vector2 m_boardDimensions;
    std::array<std::array<Tile, 8>, 8> m_board;

    
    public:
    Board();

    void SetTile(Vector2 position, Tile::Type type);

    void Render() const;

    private:
    void AddBoundaries();
};