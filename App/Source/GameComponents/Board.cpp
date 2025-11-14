#include <fstream>
#include <stdexcept>

#include "Board.h"
#include "Core/Application.h"
#include "Serialization/JsonConverters.hpp"
#include "Tile_json.hpp"
#include "UIComponents/Enums.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Board::Board()
    : Grid(UIComponents::Grid<Tile>(Vector2Ex<size_t>(8, 8), Vector2Ex<float>(50, 50), Vector2Ex<float>(10, 10),
                                    UIComponents::AnchorPoint::TOP_LEFT, Vector2Ex<float>(0, 0), Tile::Type::Path,
                                    Vector2Ex<float>(0, 0), Vector2Ex<float>(50, 50)))
{
    // AddBoundaries();

    // SetTileType({5, 2}, Tile::Type::Wall);
    // SetTileType({5, 3}, Tile::Type::Wall);
    // SetTileType({4, 2}, Tile::Type::Wall);

    // SetTileType({2, 5}, Tile::Type::Wall);
    // SetTileType({2, 4}, Tile::Type::Wall);
    // SetTileType({2, 3}, Tile::Type::Wall);
    // SetTileType({2, 2}, Tile::Type::Wall);

    // SetTileType({3, 5}, Tile::Type::Wall);
    // SetTileType({4, 5}, Tile::Type::Wall);
    // SetTileType({5, 5}, Tile::Type::Wall);

    // SetTileType({3, 4}, Tile::Type::Wall);
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
