#pragma once
#include "engine/serialization/json_converters.hpp"
#include "engine/serialization/json_helpers.hpp"
#include "game/components/tile.h"
#include "game/serialization/pellet_json.hpp"
#include "tile_type_json.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

inline void to_json(json& j, const Tile& v)
{
    to_json(j, static_cast<const ui::GridTile&>(v));

    j["type"] = v.m_type;
    j["pellet"] = v.m_pellet;
}

inline void from_json(const json& j, Tile& v)
{
    serialization::require_object(j, "Tile");

    from_json(j, static_cast<ui::GridTile&>(v));

    serialization::get_required_field(j, "type", v.m_type, "Tile");
    serialization::get_required_field(j, "pellet", v.m_pellet, "Tile");
}