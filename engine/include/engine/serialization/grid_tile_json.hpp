#pragma once

#include "engine/serialization/renderable_object_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/grid_tile.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using namespace ui;
using json = nlohmann::json;

namespace ui
{

inline void to_json(json& j, const GridTile& tile)
{
    to_json(j, static_cast<const RenderableObject&>(tile));
    j["dimensions"] = tile.m_dimensions;
}

inline void from_json(const json& j, GridTile& tile)
{
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize GridTile: JSON is not an object.");

    // Deserialize base fields first. This will throw a detailed exception on failure.
    from_json(j, static_cast<RenderableObject&>(tile));

    try
    {
        // Then deserialize the derived class's fields.
        j.at("dimensions").get_to(tile.m_dimensions);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize GridTile.dimensions: " + std::string(e.what()));
    }
}

} // namespace ui
