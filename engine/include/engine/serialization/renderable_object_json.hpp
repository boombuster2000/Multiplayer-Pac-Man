#pragma once
#include "engine/serialization/enums_json.hpp"
#include "engine/serialization/json_helpers.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/renderable_object.h"
#include <nlohmann/json.hpp>
#include <string>

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
    serialization::require_object(j, "RenderableObject");
    serialization::get_required_field(j, "worldOrigin", obj.m_worldOrigin, "RenderableObject", 500);
    serialization::get_required_field(j, "objectOrigin", obj.m_objectOrigin, "RenderableObject", 501);
    serialization::get_required_field(j, "visible", obj.m_visible, "RenderableObject", 502);
}

} // namespace ui
