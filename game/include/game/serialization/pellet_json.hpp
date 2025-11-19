#pragma once
#include "engine/serialization/renderable_object_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "game/components/pellet.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(Pellet::Type, {
                                               {Pellet::Type::NORMAL, "normal"},
                                               {Pellet::Type::SUPER, "super"},
                                               {Pellet::Type::NONE, "none"},
                                           })

inline void to_json(json& j, const Pellet& pellet)
{
    // Serialize base part first
    json base;
    to_json(base, static_cast<const ui::RenderableObject&>(pellet));

    j = base;
    j["dimensions"] = pellet.GetDimensions();
    j["type"] = pellet.GetType();
}

inline void from_json(const json& j, Pellet& pellet)
{
    // Deserialize base fields
    from_json(j, static_cast<ui::RenderableObject&>(pellet));

    if (j.contains("dimensions"))
    {
        Vector2Ex<float> dims = j.at("dimensions").get<Vector2Ex<float>>();
        pellet.radius = dims.x;
    }

    if (j.contains("type"))
    {
        pellet.m_type = j.at("type").get<Pellet::Type>();
    }
}
