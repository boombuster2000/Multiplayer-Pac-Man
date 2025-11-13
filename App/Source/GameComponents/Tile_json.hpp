#pragma once
#include "Tile.h"
#include "TileType_json.hpp"
#include "Serialization/JsonConverters.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

inline void to_json(json &j, const Tile&v) 
{ 
    json base;
    to_json(base, static_cast<const UIComponents::GridTile &>(v));

    j = base;
    j["type"] = v.GetType();

}

inline void from_json(const json &j, Tile &v) {
    
    from_json(j, static_cast<UIComponents::GridTile &>(v));

    Tile::Type type = j.at("type").get<Tile::Type>();
    v.SetType(type);
}