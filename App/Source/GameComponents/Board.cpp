#include <fstream>
#include <stdexcept>

#include "Board.h"
#include "Core/Application.h"
#include "Pellet.h"
#include "Serialization/JsonConverters.hpp"
#include "Tile_json.hpp"
#include "UIComponents/Enums.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Board::Board()
    : Grid(UIComponents::Grid<Tile>(Vector2Ex<size_t>(14, 14), Vector2Ex<float>(50, 50), Vector2Ex<float>(10, 10),
                                    UIComponents::AnchorPoint::TOP_LEFT, Vector2Ex<float>(0, 0), Tile::Type::Path,
                                    Pellet::Type::NORMAL, Vector2Ex<float>(0, 0), Vector2Ex<float>(50, 50)))
{
    AddBoundaries();

    // Row 2 - top horizontal walls
    for (int x : {2, 4, 5, 6, 7, 8, 9, 10, 11})
        SetTileType({x, 2}, Tile::Type::Wall);

    // Row 3 - vertical pillars
    for (int x : {2, 11})
        SetTileType({x, 3}, Tile::Type::Wall);

    // Row 4 - inner walls with gaps
    for (int x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 4}, Tile::Type::Wall);

    // Row 5 - ghost house top
    for (int x : {0, 2, 4, 11})
        SetTileType({x, 5}, Tile::Type::Wall);

    // Row 6 - ghost house sides
    for (int x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 6}, Tile::Type::Wall);

    // Row 7 - ghost house sides
    for (int x : {0, 2, 4, 5, 6, 7, 9, 11})
        SetTileType({x, 7}, Tile::Type::Wall);

    // Row 8 - ghost house bottom
    for (int x : {0, 2, 4, 9, 11})
        SetTileType({x, 8}, Tile::Type::Wall);

    // Row 9 - inner walls with gaps
    for (int x : {2, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 9}, Tile::Type::Wall);

    // Row 10 - vertical pillars
    for (int x : {2, 11})
        SetTileType({x, 10}, Tile::Type::Wall);

    // Row 11 - bottom horizontal walls
    for (int x : {2, 3, 4, 5, 6, 7, 8, 9, 11})
        SetTileType({x, 11}, Tile::Type::Wall);
}

Board::Board(const std::string &filename)
{
    *this = Board::LoadFromFile(filename);
}

void Board::SetTileType(const Vector2Ex<int> &index, const Tile::Type &type)
{
    Grid::GetTile(index).SetType(type);
}

void Board::SaveToFile(const std::string &filename) const
{
    json j = *this;

    std::ofstream file(filename);
    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
}

Board Board::LoadFromFile(const std::string &filename)
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

void Board::AddBoundaries()
{
    const Vector2Ex<size_t> &boardSize = GetGridSize();

    for (int x = 0; x < boardSize.x; x++)
    {
        GetTile(0, x).SetType(Tile::Type::Wall);
        GetTile(boardSize.y - 1, x).SetType(Tile::Type::Wall);
    }

    for (int y = 0; y < boardSize.y; y++)
    {
        GetTile(y, 0).SetType(Tile::Type::Wall);
        GetTile(y, boardSize.x - 1).SetType(Tile::Type::Wall);
    }
}
