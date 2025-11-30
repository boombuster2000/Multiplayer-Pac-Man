#pragma once
#include "engine/serialization/json_helpers.hpp"
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
    to_json(j, static_cast<const ui::RenderableObject&>(pellet));

    j["dimensions"] = Vector2Ex<float>(pellet.radius, pellet.radius);
    j["type"] = pellet.m_type;
}

inline void from_json(const json& j, Pellet& pellet)
{
    serialization::require_object(j, "Pellet");

    // Deserialize base fields
    from_json(j, static_cast<ui::RenderableObject&>(pellet));

    Vector2Ex<float> dims;
    serialization::get_required_field(j, "dimensions", dims, "Pellet");
    pellet.radius = dims.x;

    serialization::get_required_field(j, "type", pellet.m_type, "Pellet");
}
