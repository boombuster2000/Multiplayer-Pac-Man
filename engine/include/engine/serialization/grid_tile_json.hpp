#pragma once

#include "engine/serialization/renderable_object_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/grid_tile.h"
#include <nlohmann/json.hpp>

using namespace ui;
using json = nlohmann::json;

namespace ui
{

inline void to_json(json& j, const GridTile& tile)
{
    // Serialize base part first
    json base;
    to_json(base, static_cast<const RenderableObject&>(tile));

    j = base;
    j["dimensions"] = tile.GetDimensions();
}

inline void from_json(const json& j, GridTile& tile)
{
    // Deserialize base fields
    from_json(j, static_cast<RenderableObject&>(tile));

    if (j.contains("dimensions"))
        tile.SetDimensions(j.at("dimensions").get<Vector2Ex<float>>());
}

} // namespace ui
