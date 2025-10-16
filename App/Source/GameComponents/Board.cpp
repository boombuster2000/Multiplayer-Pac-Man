#include "Board.h"
#include "Core/Application.h"
#include "UIComponents/Enums.h"

Board::Board()
    : m_grid(UIComponents::Grid<Tile>(
          Vector2Ex<size_t>(8, 8),
          Vector2Ex<int>(50, 50),
          Vector2Ex<int>(10, 10),
          UIComponents::AnchorPoint::MIDDLE,
          Vector2Ex<int>(0, 0),
          Tile::Type::Path,
          Vector2Ex<int>(0, 0),
          Vector2Ex<int>(50, 50))),
      m_boardDimensions(8, 8)
{
    AddBoundaries();

    SetTile({5, 2}, Tile::Type::Wall);
    SetTile({5, 3}, Tile::Type::Wall);
    SetTile({4, 2}, Tile::Type::Wall);

    SetTile({2, 5}, Tile::Type::Wall);
    SetTile({2, 4}, Tile::Type::Wall);
    SetTile({2, 3}, Tile::Type::Wall);
    SetTile({2, 2}, Tile::Type::Wall);

    SetTile({3, 5}, Tile::Type::Wall);
    SetTile({4, 5}, Tile::Type::Wall);
    SetTile({5, 5}, Tile::Type::Wall);

    SetTile({3, 4}, Tile::Type::Wall);
}

void Board::SetTile(Vector2Ex<int> position, Tile::Type type)
{
    m_grid[position.y][position.x].SetType(type);
}

Vector2Ex<int> Board::ConvertPositionToIndex(Vector2Ex<int> position)
{

    // TODO make grid class for board.
    return Vector2();
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
        m_grid.At(y, 0).SetType(Tile::Type::Wall);
        m_grid[y][m_boardDimensions.x - 1].SetType(Tile::Type::Wall);
    }
}

void Board::Render(Vector2Ex<int> offset) const
{
    Vector2Ex<int> cursor = {0, 0};

    for (const auto &row : m_grid)
    {
        for (const auto &tile : row)
        {
            tile.Render();
            cursor.x += 50;
        }

        cursor.x = 0;
        cursor.y += 50;
    }
}
