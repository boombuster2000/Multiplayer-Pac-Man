#include "GameLayer.h"
#include "raylib.h"
#include "Core/Application.h"
#include "UIComponents/Enums.h"


GameLayer::GameLayer()
: m_board(Board()), m_pacman(PacMan({50,50}, {50,50}))
{
}

void GameLayer::OnUpdate(float ts)
{
    using enum UIComponents::Direction;

    if (IsKeyDown(KEY_W))
        m_pacman.Move(UP, 5);
    
    if (IsKeyDown(KEY_S))
        m_pacman.Move(DOWN, 5);
    
    if (IsKeyDown(KEY_A))
        m_pacman.Move(LEFT, 5);

    if (IsKeyDown(KEY_D))
        m_pacman.Move(RIGHT, 5);
}

void GameLayer::OnRender()
{
    ClearBackground(WHITE);
    m_board.Render();
    m_pacman.Render();
}
