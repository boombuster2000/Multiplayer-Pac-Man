#pragma once
#include "engine/serialization/enums_json.hpp"
#include "engine/serialization/vector2ex_json.hpp"
#include "engine/ui/renderable_object.h"
#include <nlohmann/json.hpp>

using namespace ui;
using json = nlohmann::json;

namespace ui
{

// Serialize only base data (not pure virtual parts)
inline void to_json(json& j, const RenderableObject& obj)
{
    j = json{
        {"worldOrigin", obj.GetWorldOrigin()}, {"objectOrigin", obj.GetObjectOrigin()}, {"visible", obj.IsVisible()}};
}

inline void from_json(const json& j, RenderableObject& obj)
{
    obj.SetPosition(j.at("worldOrigin").get<Vector2Ex<float>>());
    obj.SetOrigin(j.at("objectOrigin").get<Vector2Ex<float>>());
    obj.SetVisibility(j.at("visible").get<bool>());
}

} // namespace ui
