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
          Vector2Ex<float>(50, 50)))
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
    Grid::GetTile(index).SetType(type);
}

void Board::AddBoundaries()
{
    const Vector2Ex<size_t>& boardSize = GetGridSize();

    for (int x = 0; x < boardSize.x; x++)
    {
        GetTile(0,x).SetType(Tile::Type::Wall);
        GetTile(boardSize.y - 1,x).SetType(Tile::Type::Wall);
    }

    for (int y = 0; y < boardSize.y; y++)
    {
        GetTile(y,0).SetType(Tile::Type::Wall);
        GetTile(y,boardSize.x - 1).SetType(Tile::Type::Wall);
    }
}
