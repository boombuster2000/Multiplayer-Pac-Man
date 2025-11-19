#include "game/components/profile.h"

Profile::Profile(std::string username) : m_username(username)
{
}
void Profile::SetUsername(const std::string& username)
{
    m_username = username;
}
std::string Profile::GetUsername() const
{
    return m_username;
}