#pragma once
#include "engine/serialization/json_helpers.hpp"
#include "game/utils/highscore_utils.h"
#include <nlohmann/json.hpp>
#include <string_view>
#include <unordered_map>

using game::highscore_utils::HighscoreMap;
using nlohmann::json;

class Profile
{
  private:
    std::string m_username;
    HighscoreMap m_personalHighscores{};

  public:
    Profile() = default;
    explicit Profile(std::string_view username);

    std::string_view GetUsername() const;

    HighscoreMap GetPersonalHighscores() const;
    void UpdateHighScore(std::string_view boardName, const int points);
    void Save() const;

    friend void to_json(json& j, const Profile& profile);
    friend void from_json(const json& j, Profile& profile);
};