#pragma once
#include "engine/serialization/renderable_object_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "game/components/pellet.h"
#include <nlohmann/json.hpp>
#include <stdexcept>

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
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize Pellet: JSON is not an object.");

    // Deserialize base fields
    from_json(j, static_cast<ui::RenderableObject&>(pellet));

    try
    {
        Vector2Ex<float> dims = j.at("dimensions").get<Vector2Ex<float>>();
        pellet.radius = dims.x;
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Pellet.dimensions: " + std::string(e.what()));
    }

    try
    {
        j.at("type").get_to(pellet.m_type);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Pellet.type: " + std::string(e.what()));
    }
}
