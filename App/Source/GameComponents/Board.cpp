#include "Board.h"
#include "Core/Application.h"
#include "UIComponents/Enums.h"

Board::Board()
    : Grid(UIComponents::Grid<Tile>(
          Vector2Ex<size_t>(8, 8),
          Vector2Ex<float>(50, 50),
          Vector2Ex<float>(10, 10),
          UIComponents::AnchorPoint::TOP_LEFT,
          Vector2Ex<float>(0, 0),
          Tile::Type::Path,
          Vector2Ex<float>(0, 0),
          Vector2Ex<float>(50, 50))),
      m_boardDimensions(8, 8)
{
    AddBoundaries();

    SetTileType({5, 2}, Tile::Type::Wall);
    SetTileType({5, 3}, Tile::Type::Wall);
    SetTileType({4, 2}, Tile::Type::Wall);

    SetTileType({2, 5}, Tile::Type::Wall);
    SetTileType({2, 4}, Tile::Type::Wall);
    SetTileType({2, 3}, Tile::Type::Wall);
    SetTileType({2, 2}, Tile::Type::Wall);

    SetTileType({3, 5}, Tile::Type::Wall);
    SetTileType({4, 5}, Tile::Type::Wall);
    SetTileType({5, 5}, Tile::Type::Wall);

    SetTileType({3, 4}, Tile::Type::Wall);
}

void Board::SetTileType(const Vector2Ex<int> &index, const Tile::Type &type)
{
    m_grid[index.y][index.x].SetType(type);
}

void Board::AddBoundaries()
{
    for (int x = 0; x < m_boardDimensions.x; x++)
    {
        m_grid[0][x].SetType(Tile::Type::Wall);
        m_grid[m_boardDimensions.y - 1][x].SetType(Tile::Type::Wall);
    }

    for (int y = 0; y < m_boardDimensions.y; y++)
    {
        m_grid[y][0].SetType(Tile::Type::Wall);
        m_grid[y][m_boardDimensions.x - 1].SetType(Tile::Type::Wall);
    }
}
