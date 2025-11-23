#pragma once
#include "engine/serialization/json_converters.hpp"
#include "game/components/tile.h"
#include "game/serialization/pellet_json.hpp"
#include "tile_type_json.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;
using namespace ui;

inline void to_json(json& j, const Tile& v)
{
    to_json(j, static_cast<const GridTile&>(v));

    j["type"] = v.m_type;
    j["pellet"] = v.m_pellet;
}

inline void from_json(const json& j, Tile& v)
{
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize Tile: JSON is not an object.");

    from_json(j, static_cast<GridTile&>(v));

    try
    {
        j.at("type").get_to(v.m_type);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Tile.type: " + std::string(e.what()));
    }

    try
    {
        j.at("pellet").get_to(v.m_pellet);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Tile.pellet: " + std::string(e.what()));
    }
}