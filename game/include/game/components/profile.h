#pragma once
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Profile
{
  private:
    std::string m_username;

  public:
    Profile() = default;
    Profile(std::string username);

    std::string GetUsername() const;

    friend void to_json(json&, const Profile&);
    friend void from_json(const json&, Profile&);
};