#include "Board.h"
#include "Core/Application.h"

Board::Board()
    : m_board{}
    , m_boardDimensions({8,8})
{
    for (auto& row : m_board)
    {
        for (auto& tile : row)
        {
            tile = Tile(Tile::Type::Path);
        }
    }

    AddBoundaries();

    SetTile({5,2}, Tile::Type::Wall);
    SetTile({5,3}, Tile::Type::Wall);
    SetTile({4,2}, Tile::Type::Wall);

    SetTile({2,5}, Tile::Type::Wall);
    SetTile({2,4}, Tile::Type::Wall);
    SetTile({2,3}, Tile::Type::Wall);
    SetTile({2,2}, Tile::Type::Wall);

    SetTile({3,5}, Tile::Type::Wall);
    SetTile({4,5}, Tile::Type::Wall);
    SetTile({5,5}, Tile::Type::Wall);

    SetTile({3,4}, Tile::Type::Wall);

    

}

void Board::SetTile(Vector2 position, Tile::Type type)
{
    m_board[position.y][position.x].SetType(type);
}

void Board::AddBoundaries()
{
    for (int x = 0; x<m_boardDimensions.x; x++)
    {    
        m_board[0][x].SetType(Tile::Type::Wall);
        m_board[m_boardDimensions.y-1][x].SetType(Tile::Type::Wall);
    }  
    
    for (int y = 0; y<m_boardDimensions.y; y++)
    {    
        m_board[y][0].SetType(Tile::Type::Wall);
        m_board[y][m_boardDimensions.x-1].SetType(Tile::Type::Wall);
    }
}


void Board::Render() const
{
    Vector2 cursor = {0,0};

    for (const auto& row : m_board)
    {
        for (const auto& tile : row)
        {
            tile.Render(cursor, {50,50});
            cursor.x += 50;
        }

        cursor.x = 0;
        cursor.y += 50;
    }
}
