#pragma once
#include "Enums_json.hpp"
#include "UIComponents/RenderableObject.h"
#include "Vector2Ex_json.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace UIComponents
{

// Serialize only base data (not pure virtual parts)
inline void to_json(json &j, const RenderableObject &obj)
{
    j = json{
        {"worldOrigin", obj.GetWorldOrigin()}, {"objectOrigin", obj.GetObjectOrigin()}, {"visible", obj.IsVisible()}};
}

inline void from_json(const json &j, RenderableObject &obj)
{
    obj.SetPosition(j.at("worldOrigin").get<Vector2Ex<float>>());
    obj.SetOrigin(j.at("objectOrigin").get<Vector2Ex<float>>());
    obj.SetVisibility(j.at("visible").get<bool>());
}

} // namespace UIComponents
