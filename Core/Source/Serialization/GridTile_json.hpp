#pragma once
#include <nlohmann/json.hpp>
#include "UIComponents/GridTile.h"
#include "RenderableObject_json.hpp"
#include "Vector2Ex_json.hpp"

using nlohmann::json;

namespace UIComponents {

    inline void to_json(json &j, const GridTile &tile)
    {
        // Serialize base part first
        json base;
        to_json(base, static_cast<const RenderableObject &>(tile));

        j = base;
        j["dimensions"] = tile.GetDimensions();
    }

    inline void from_json(const json &j, GridTile &tile)
    {
        // Deserialize base fields
        from_json(j, static_cast<RenderableObject &>(tile));

        if (j.contains("dimensions"))
            tile.SetDimensions(j.at("dimensions").get<Vector2Ex<float>>());
    }

}
