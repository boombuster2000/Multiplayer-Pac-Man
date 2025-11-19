#pragma once
#include "engine/serialization/json_converters.hpp"
#include "game/components/tile.h"
#include "tile_type_json.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace ui;

inline void to_json(json& j, const Tile& v)
{
    json base;
    to_json(base, static_cast<const GridTile&>(v));

    j = base;
    j["type"] = v.GetType();
}

inline void from_json(const json& j, Tile& v)
{

    from_json(j, static_cast<GridTile&>(v));

    Tile::Type type = j.at("type").get<Tile::Type>();
    v.SetType(type);
}