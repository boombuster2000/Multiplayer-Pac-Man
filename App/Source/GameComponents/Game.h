#pragma once
#include "Tile.h"
#include "Board.h"
#include "Core/TexturesManager.h"

class Game
{
    private:
    Board m_board;

    public:
    Game();
    ~Game();

    void Render() const;
};