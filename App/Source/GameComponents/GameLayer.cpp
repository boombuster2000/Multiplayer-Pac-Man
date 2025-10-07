#include "GameLayer.h"
#include "raylib.h"


GameLayer::GameLayer()
: m_game(Game())
{
}

void GameLayer::OnUpdate(float ts)
{
}

void GameLayer::OnRender()
{
    ClearBackground(WHITE);
    m_game.Render();
}
