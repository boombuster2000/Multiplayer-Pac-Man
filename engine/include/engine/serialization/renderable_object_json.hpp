#pragma once
#include "engine/serialization/enums_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/renderable_object.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using namespace ui;
using json = nlohmann::json;

namespace ui
{

// Serialize only base data (not pure virtual parts)
inline void to_json(json& j, const RenderableObject& obj)
{
    j["worldOrigin"] = obj.m_worldOrigin;
    j["objectOrigin"] = obj.m_objectOrigin;
    j["visible"] = obj.m_visible;
}

inline void from_json(const json& j, RenderableObject& obj)
{
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize RenderableObject: JSON is not an object.");

    try
    {
        j.at("worldOrigin").get_to(obj.m_worldOrigin);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize RenderableObject.worldOrigin: " + std::string(e.what()));
    }

    try
    {
        j.at("objectOrigin").get_to(obj.m_objectOrigin);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize RenderableObject.objectOrigin: " + std::string(e.what()));
    }

    try
    {
        j.at("visible").get_to(obj.m_visible);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize RenderableObject.visible: " + std::string(e.what()));
    }
}

} // namespace ui
