#pragma once
#include "engine/core/vector2ex.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;

// Vector2Ex
template <NumberLike T> inline void to_json(json& j, const Vector2Ex<T>& v)
{
    j = {{"x", v.x}, {"y", v.y}};
}

template <NumberLike T> inline void from_json(const json& j, Vector2Ex<T>& v)
{
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
}