#include "Board.h"

Board::Board(std::array<std::array<Tile::Type, 8>, 8> board)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            m_board[i][j] = Tile(board[i][j]);
        }
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
