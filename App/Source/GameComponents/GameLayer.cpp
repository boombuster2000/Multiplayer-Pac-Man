#include "GameLayer.h"
#include "raylib.h"
#include "Core/Application.h"
#include "UIComponents/Enums.h"


GameLayer::GameLayer()
: m_board(Board()), m_pacman(PacMan({50,50}, {50,50}, 3))
{
}

void GameLayer::HandleKeyPresses()
{
    using enum UIComponents::Direction;
    if (IsKeyPressed(KEY_W))
        m_pacman.SetDirection(UP);
    
    if (IsKeyPressed(KEY_S))
        m_pacman.SetDirection(DOWN);
    
    if (IsKeyPressed(KEY_A))
        m_pacman.SetDirection(LEFT);

    if (IsKeyPressed(KEY_D))
        m_pacman.SetDirection(RIGHT);
}

void GameLayer::OnUpdate(float ts)
{
    HandleKeyPresses();
    m_pacman.Update();
}

void GameLayer::OnRender()
{
    ClearBackground(WHITE);
    m_board.Render();
    m_pacman.Render();
}
