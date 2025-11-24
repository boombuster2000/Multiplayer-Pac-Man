#pragma once

#include "engine/serialization/json_helpers.hpp"
#include "engine/serialization/renderable_object_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/grid_tile.h"
#include <nlohmann/json.hpp>
#include <string>

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
    serialization::require_object(j, "GridTile");

    // Deserialize base fields first. This will throw a detailed exception on failure.
    from_json(j, static_cast<RenderableObject&>(tile));

    // Then deserialize the derived class's fields.
    serialization::get_required_field(j, "dimensions", tile.m_dimensions, "GridTile", 500);
}

} // namespace ui
