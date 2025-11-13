#pragma once
#include <string>
#include "Tile.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

inline void to_json(json &j, const Tile::Type&v) 
{ 
    switch (v)
    {
    case Tile::Type::Wall:
        j = "Wall";
        break;
    case Tile::Type::Path:
        j = "Path";
        break;
    case Tile::Type::None:
        j = "None";
        break;
    }
}

inline void from_json(const json &j, Tile::Type &v) {
    
    const auto& s = j.get<std::string>();
    
    if (s == "Wall")
        v = Tile::Type::Wall;

    else if (s == "Path")
        v = Tile::Type::Path;

    else if (s == "None")
        v = Tile::Type::None;
}