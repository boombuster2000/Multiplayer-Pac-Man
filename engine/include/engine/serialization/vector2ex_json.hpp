#pragma once
#include "engine/core/vector2ex.h"
#include "engine/serialization/json_helpers.hpp"
#include <nlohmann/json.hpp>
#include <string>

using nlohmann::json;

// Vector2Ex
template <NumberLike T>
inline void to_json(json& j, const Vector2Ex<T>& v)
{
    j = {{"x", v.x}, {"y", v.y}};
}

template <NumberLike T>
inline void from_json(const json& j, Vector2Ex<T>& v)
{
    serialization::require_object(j, "Vector2Ex");
    serialization::get_required_field(j, "x", v.x, "Vector2Ex", 500);
    serialization::get_required_field(j, "y", v.y, "Vector2Ex", 501);
}
