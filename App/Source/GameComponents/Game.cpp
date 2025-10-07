#include "Game.h"
#include "raylib.h"
#include "Core/Application.h"

Game::Game()
    : m_board({{
        {{Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Path, Tile::Type::Path, Tile::Type::Path, Tile::Type::Path, Tile::Type::Path, Tile::Type::Path, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Path, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Path, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Path, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Path, Tile::Type::Path, Tile::Type::Wall, Tile::Type::Path, Tile::Type::Path, Tile::Type::Path, Tile::Type::Wall}},
        {{Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall, Tile::Type::Wall}}
    }})
{

    Core::Application::GetTexturesManager()->AddTexture("wall", "./Resources/Textures/Wall.png");
    Core::Application::GetTexturesManager()->AddTexture("path", "./Resources/Textures/Path.png");
}

Game::~Game() = default;

void Game::Render() const
{
    Vector2 cursor = {0,0};
    m_board.Render();
}