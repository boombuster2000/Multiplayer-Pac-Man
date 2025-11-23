#pragma once
#include "engine/core/vector2ex.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

// Vector2Ex
template <NumberLike T>
inline void to_json(json& j, const Vector2Ex<T>& v)
{
    j = {{"x", v.x}, {"y", v.y}};
}

template <NumberLike T>
inline void from_json(const json& j, Vector2Ex<T>& v)
{
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize Vector2Ex: JSON is not an object.");

    try
    {
        j.at("x").get_to(v.x);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Vector2Ex.x: " + std::string(e.what()));
    }

    try
    {
        j.at("y").get_to(v.y);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Vector2Ex.y: " + std::string(e.what()));
    }
}
