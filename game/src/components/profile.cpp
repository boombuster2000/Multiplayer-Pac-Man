#include "game/components/profile.h"
#include "game/serialization/json_converters.hpp"
#include "profile.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

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

void Profile::UpdateHighScore(const std::string& boardName, const int points)
{
    if (boardName.empty())
        return;

    if (points <= 0)
        return;

    bool newHighScore = false;
    if (m_personalHighscores.contains(boardName))
    {
        if (m_personalHighscores.at(boardName) < points)
        {
            m_personalHighscores[boardName] = points;
            newHighScore = true;
        }
    }
    else
    {
        m_personalHighscores[boardName] = points;
        newHighScore = true;
    }

    if (newHighScore)
    {
        Save();
    }
}

void Profile::Save()
{
    using nlohmann::json;
    json profile_json = *this;
    std::ofstream o("profiles/" + GetUsername() + ".json");
    o << std::setw(4) << profile_json << std::endl;
}
