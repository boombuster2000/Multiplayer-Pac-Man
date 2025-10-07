#pragma once
#include "Tile.h"
#include <array>

class Board
{
    private:
    std::array<std::array<Tile, 8>, 8> m_board;

    public:
    Board(std::array<std::array<Tile::Type, 8>, 8> board);

    void Render() const;
};