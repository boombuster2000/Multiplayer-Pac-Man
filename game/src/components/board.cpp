#include <fstream>
#include <stdexcept>

#include "engine/core/application.h"
#include "engine/serialization/json_converters.hpp"
#include "engine/ui/enums.h"
#include "game/components/board.h"
#include "game/components/pellet.h"
#include "game/serialization/json_converters.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Board::Board()
    : Grid(Vector2Ex<size_t>(14, 14), Vector2Ex<float>(50, 50),
           Vector2Ex<float>(static_cast<float>(GetScreenWidth()) / 2, static_cast<float>(GetScreenHeight()) / 2),
           ui::AnchorPoint::MIDDLE, Vector2Ex<float>(0, 0), Tile::Type::PATH, Pellet::Type::NORMAL,
           Vector2Ex<float>(0, 0), Vector2Ex<float>(50, 50)),
      m_name("built-in")
{
    addBoundaries();

    // Row 2 - top horizontal walls
    for (int x : {2, 4, 5, 6, 7, 8, 9, 10, 11})
        SetTileType({x, 2}, Tile::Type::WALL);

    // Row 3 - vertical pillars
    for (int x : {2, 11})
        SetTileType({x, 3}, Tile::Type::WALL);

    // Row 4 - inner walls with gaps
    for (int x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 4}, Tile::Type::WALL);

    // Row 5 - ghost house top
    for (int x : {0, 2, 4, 11})
        SetTileType({x, 5}, Tile::Type::WALL);

    // Row 6 - ghost house sides
    for (int x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 6}, Tile::Type::WALL);

    // Row 7 - ghost house sides
    for (int x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 7}, Tile::Type::WALL);

    // Row 8 - ghost house bottom
    for (int x : {0, 2, 4, 9, 11})
        SetTileType({x, 8}, Tile::Type::WALL);

    // Row 9 - inner walls with gaps
    for (int x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 9}, Tile::Type::WALL);

    // Row 10 - vertical pillars
    for (int x : {2, 11})
        SetTileType({x, 10}, Tile::Type::WALL);

    // Row 11 - bottom horizontal walls
    for (int x : {2, 3, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 11}, Tile::Type::WALL);
}

Board::Board(const std::string& filename)
{
    *this = Board::LoadFromFile(filename);
}

std::string Board::GetName() const
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

Board Board::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    json j;
    file >> j;
    file.close();
    Board board = j.get<Board>();
    return board;
}

void Board::addBoundaries()
{
    const Vector2Ex<size_t>& boardSize = GetGridSize();

    for (int x = 0; x < boardSize.x; x++)
    {
        GetTile(0, x).SetType(Tile::Type::WALL);
        GetTile(boardSize.y - 1, x).SetType(Tile::Type::WALL);
    }

    for (int y = 0; y < boardSize.y; y++)
    {
        GetTile(y, 0).SetType(Tile::Type::WALL);
        GetTile(y, boardSize.x - 1).SetType(Tile::Type::WALL);
    }
}

Vector2Ex<float> Board::GetPlayerSpawnPoint() const
{
    return GetPositionFromIndex({1, 1});
}
