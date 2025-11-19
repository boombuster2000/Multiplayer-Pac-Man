#pragma once
#include <string>

class Profile
{
  private:
    std::string m_username;

  public:
    Profile(std::string username);

    void SetUsername(const std::string& username);
    std::string GetUsername() const;
};