
#pragma once
#include <array>
class Game
{
    private:
    std::array<std::array<char, 8>, 8> m_board;

    public:
    Game();
    ~Game();

    void Render() const;
};