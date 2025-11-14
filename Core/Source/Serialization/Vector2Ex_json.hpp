#pragma once
#include <nlohmann/json.hpp>

#include "DataTypes/Vector2Ex.h"

using nlohmann::json;

// Vector2Ex
template <NumberLike T> inline void to_json(json &j, const Vector2Ex<T> &v)
{
    j = {{"x", v.x}, {"y", v.y}};
}

template <NumberLike T> inline void from_json(const json &j, Vector2Ex<T> &v)
{
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
}