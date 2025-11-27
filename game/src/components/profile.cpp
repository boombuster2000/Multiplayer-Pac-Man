#include "game/components/profile.h"
#include "game/serialization/json_converters.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

Profile::Profile(std::string_view username) : m_username(username)
{
}

std::string Profile::GetUsername() const
{
    return m_username;
}
HighscoreMap Profile::GetPersonalHighscores() const
{
    return m_personalHighscores;
}

void Profile::UpdateHighScore(std::string_view boardName, const int points)
{
    if (boardName.empty())
        return;

    if (points <= 0)
        return;

    bool newHighScore = false;

    if (auto it = m_personalHighscores.find(boardName); it != m_personalHighscores.end())
    {
        if (it->second < points)
        {
            it->second = points;
            newHighScore = true;
        }
    }
    else
    {
        m_personalHighscores.emplace(boardName, points);
        newHighScore = true;
    }

    if (newHighScore)
    {
        Save();
    }
}

void Profile::Save() const
{
    using nlohmann::json;
    json profile_json = *this;
    std::ofstream o("profiles/" + GetUsername() + ".json");
    o << std::setw(4) << profile_json << std::endl;
}
