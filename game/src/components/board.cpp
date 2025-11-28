#include "game/components/board.h"
#include "engine/serialization/json_converters.hpp"
#include "engine/ui/enums.h"
#include "game/components/pellet.h"
#include "game/game_application.h"
#include "game/serialization/json_converters.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>


Board::Board()
    : Grid(Vector2Ex<size_t>(14, 14), Vector2Ex<float>(50, 50),
           Vector2Ex<float>(static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) / 2),
           ui::AnchorPoint::MIDDLE, Vector2Ex<float>(0, 0), Tile::Type::PATH, Pellet::Type::NORMAL,
           Vector2Ex<float>(0, 0), Vector2Ex<float>(50, 50)),
      m_name("test-file")
{
    using enum Tile::Type;
    addBoundaries();

    // Row 2 - top horizontal walls
    for (int x : {2, 4, 5, 6, 7, 8, 9, 10, 11})
        SetTileType({x, 2}, WALL);

    // Row 3 - vertical pillars
    for (int x : {2, 11})
        SetTileType({x, 3}, WALL);

    // Row 4 - inner walls with gaps
    for (int x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 4}, WALL);

    // Row 5 - ghost house top
    for (int x : {0, 2, 4, 11})
        SetTileType({x, 5}, WALL);

    // Row 6 - ghost house sides
    for (int x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 6}, WALL);

    // Row 7 - ghost house sides
    for (int x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 7}, WALL);

    // Row 8 - ghost house bottom
    for (int x : {0, 2, 4, 9, 11})
        SetTileType({x, 8}, WALL);

    // Row 9 - inner walls with gaps
    for (int x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 9}, WALL);

    // Row 10 - vertical pillars
    for (int x : {2, 11})
        SetTileType({x, 10}, WALL);

    // Row 11 - bottom horizontal walls
    for (int x : {2, 3, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 11}, WALL);
}

Board::Board(std::string_view filename)
{
    *this = Board::LoadFromFile(filename);
}

const std::string& Board::GetName() const
{
    return m_name;
}

void Board::SetTileType(const Vector2Ex<int>& index, const Tile::Type& type)
{
    Grid::GetTile(index).SetType(type);
}

void Board::SaveToFile() const
{
    json j = *this;

    const std::string boardFolder = "./resources/boards/";
    const std::string filename = m_name + std::string(".json");

    std::ofstream file(boardFolder + filename);
    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
}

void Board::SaveHighscoresToFile() const
{
    const std::string boardFolder = "./resources/boards/";
    const std::string filename = m_name + std::string(".json");

    Board originalBoard = Board::LoadFromFile(boardFolder + filename);

    for (const auto& [profileName, score] : m_highScores)
    {
        originalBoard.SetHighscore(profileName, score);
    }

    json j = originalBoard;

    std::ofstream file(boardFolder + filename);

    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
}

HighscoreMap Board::GetHighscores() const
{
    return m_highScores;
}

void Board::SetHighscore(std::string_view profileName, int score)
{
    if (auto it = m_highScores.find(profileName); it != m_highScores.end())
    {
        // Profile exists, update if score is higher
        if (score > it->second)
        {
            it->second = score;
        }
    }
    else
    {
        // Profile doesn't exist, insert new score
        m_highScores.emplace(profileName, score);
    }
}

Board Board::LoadFromFile(std::string_view filename)
{
    const std::string filenameStr(filename);
    std::ifstream file(filenameStr);
    if (!file.is_open())
    {
        throw std::filesystem::filesystem_error("Failed to open file", std::filesystem::path(filename.data()),
                                                std::error_code{});
    }

    json j;
    file >> j;
    file.close();
    Board board = j.get<Board>();
    board.m_highScores = game::highscore_utils::SortHighscores(board.GetHighscores());
    return board;
}

void Board::addBoundaries()
{
    using enum Tile::Type;
    const Vector2Ex<size_t>& boardSize = GetGridSize();

    for (int x = 0; x < boardSize.x; x++)
    {
        GetTile(0, x).SetType(WALL);
        GetTile(boardSize.y - 1, x).SetType(WALL);
    }

    for (int y = 0; y < boardSize.y; y++)
    {
        GetTile(y, 0).SetType(WALL);
        GetTile(y, boardSize.x - 1).SetType(WALL);
    }
}

Vector2Ex<float> Board::GetPlayerSpawnPoint() const
{
    return GetPositionFromIndex({1, 1});
}
