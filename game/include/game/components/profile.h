#pragma once
#include "engine/core/transparentStringHasher.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, int, TransparentStringHash, std::equal_to<>> HighscoreMap;
using json = nlohmann::json;

class Profile
{
  private:
    std::string m_username;
    HighscoreMap m_personalHighscores{};

  public:
    Profile() = default;
    explicit Profile(const std::string& username);

    std::string GetUsername() const;

    HighscoreMap GetPersonalHighscores() const;
    void UpdateHighScore(const std::string& boardName, const int points);
    void Save() const;

    friend void to_json(json&, const Profile&);
    friend void from_json(const json&, Profile&);
};