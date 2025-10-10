#include "GameLayer.h"
#include "raylib.h"
#include "Core/Application.h"
#include "UIComponents/Enums.h"


GameLayer::GameLayer()
: m_board(Board()), m_pacman(PacMan({50,50}, {50,50}, 3))
{
}

void GameLayer::OnUpdate(float ts)
{
    using enum UIComponents::Direction;

    if (IsKeyDown(KEY_W))
        m_pacman.QueueDirection(UP);
    
    if (IsKeyDown(KEY_S))
        m_pacman.QueueDirection(DOWN);
    
    if (IsKeyDown(KEY_A))
        m_pacman.QueueDirection(LEFT);

    if (IsKeyDown(KEY_D))
        m_pacman.QueueDirection(RIGHT);

    m_pacman.Update();
}

void GameLayer::OnRender()
{
    ClearBackground(WHITE);
    m_board.Render();
    m_pacman.Render();
}
