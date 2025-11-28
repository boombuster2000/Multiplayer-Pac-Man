#include "game/components/profile.h"
#include "game/serialization/json_converters.hpp"
#include "game/utils/file_utils.h"
#include <format>
#include <nlohmann/json.hpp>


Profile::Profile(std::string_view username) : m_username(username)
{
}

std::string_view Profile::GetUsername() const
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
    const std::filesystem::path folder = "profiles";
    const std::string filename = std::format("{}.json", GetUsername());

    json profile_json = *this;

    game::file_utils::SaveJson(profile_json, folder, filename);
}