#pragma once
#include "engine/serialization/json_helpers.hpp"
#include "game/components/profile.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

inline void to_json(json& j, const Profile& profile)
{
    j = json{{"username", profile.m_username}, {"personal_highscores", profile.m_personalHighscores}};
}

inline void from_json(const json& j, Profile& profile)
{
    serialization::require_object(j, "Profile");
    serialization::get_required_field(j, "username", profile.m_username, "Profile");
    serialization::get_required_field(j, "personal_highscores", profile.m_personalHighscores, "Profile");
}
