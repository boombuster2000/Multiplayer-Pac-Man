#pragma once
#include "game/components/profile.h"
#include <nlohmann/json.hpp>
#include <stdexcept>

using nlohmann::json;

inline void to_json(json& j, const Profile& profile)
{
    j = json{{"username", profile.m_username}};
}

inline void from_json(const json& j, Profile& profile)
{
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize Profile: JSON is not an object.");

    try
    {
        j.at("username").get_to(profile.m_username);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Profile.username: " + std::string(e.what()));
    }
}
