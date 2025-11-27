#include "game/utils/highscore_utils.h"
#include "game/components/board.h"
#include "game/components/profile.h"
#include <algorithm>
#include <vector>

namespace game
{
namespace highscore_utils
{

void SaveHighscore(::Board& board, ::Profile& profile, const int score)
{
    if (score <= 0)
        return;

    board.SetHighscore(profile.GetUsername(), score);
    board.SaveHighscoresToFile();

    profile.UpdateHighScore(board.GetName(), score);
}

HighscoreMap SortHighscores(const HighscoreMap& highscores)
{
    HighscoreMap sortedHighscores;
    // Convert unordered_map to vector of pairs
    std::vector<std::pair<std::string, int>> scoreVector(highscores.begin(), highscores.end());

    // Sort the vector based on scores in descending order
    std::ranges::sort(scoreVector, [](const auto& a, const auto& b) { return a.second > b.second; });

    // Rebuild the sorted unordered_map
    for (const auto& [key, value] : scoreVector)
    {
        sortedHighscores[key] = value;
    }

    return sortedHighscores;
}

} // namespace highscore_utils

} // namespace game