#pragma once
#include "game/components/tile.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

inline void to_json(json& j, const Tile::Type& v)
{
    switch (v)
    {
    case Tile::Type::WALL:
        j = "WALL";
        break;
    case Tile::Type::PATH:
        j = "PATH";
        break;
    case Tile::Type::NONE:
        j = "None";
        break;
    }
}

inline void from_json(const json& j, Tile::Type& v)
{

    const auto& s = j.get<std::string>();

    if (s == "WALL")
        v = Tile::Type::WALL;

    else if (s == "PATH")
        v = Tile::Type::PATH;

    else if (s == "None")
        v = Tile::Type::NONE;
}