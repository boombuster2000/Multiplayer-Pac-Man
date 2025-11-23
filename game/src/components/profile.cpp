#include "game/components/profile.h"
#include "profile.h"

Profile::Profile(std::string username) : m_username(username), m_personalHighscores()
{
}

std::string Profile::GetUsername() const
{
    return m_username;
}
std::unordered_map<std::string, int> Profile::GetPersonalHighscores() const
{
    return m_personalHighscores;
}

void Profile::SaveHighscore(const std::string& boardName, const int points)
{
    if (boardName.empty())
        return;

    if (points <= 0)
        return;

    if (m_personalHighscores.contains(boardName))
    {
        if (m_personalHighscores.at(boardName) < points)
            m_personalHighscores[boardName] = points;
    }
    else
    {
        m_personalHighscores[boardName] = points;
    }
}
