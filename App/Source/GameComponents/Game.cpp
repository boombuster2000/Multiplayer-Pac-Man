#include "Game.h"
#include "raylib.h"

Game::Game()
    : m_board{{
        {{'#', '#', '#', '#', '#', '#', '#', '#'}},
        {{'#', ' ', ' ', ' ', ' ', ' ', ' ', '#'}},
        {{'#', ' ', ' ', '#', ' ', '#', ' ', '#'}},
        {{'#', ' ', ' ', '#', ' ', '#', ' ', '#'}},
        {{'#', ' ', '#', '#', ' ', '#', ' ', '#'}},
        {{'#', ' ', ' ', '#', '#', '#', ' ', '#'}},
        {{'#', ' ', ' ', '#', ' ', ' ', ' ', '#'}},
        {{'#', '#', '#', '#', '#', '#', '#', '#'}}
    }}
{}

Game::~Game() = default;

void Game::Render() const
{
    Vector2 cursor = {0,0};

    for (const auto& row : m_board)
    {
        for (const auto& tile : row)
        {
            if (tile == ' ')
            {
                DrawRectangleV(cursor, {50,50}, GRAY);
            }
            else if (tile == '#')
            {
                DrawRectangleV(cursor, {50,50}, BLACK);
            }
            cursor.x += 50;
        }

        cursor.x = 0;
        cursor.y += 50;
    }
}