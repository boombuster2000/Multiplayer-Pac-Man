#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

class Profile
{
  private:
    std::string m_username;
    std::unordered_map<std::string, int> m_personalHighscores;

  public:
    Profile() = default;
    Profile(std::string username);

    std::string GetUsername() const;

    std::unordered_map<std::string, int> GetPersonalHighscores() const;
    void UpdateHighScore(const std::string& boardName, const int points);
    void Save();

    friend void to_json(json&, const Profile&);
    friend void from_json(const json&, Profile&);
};