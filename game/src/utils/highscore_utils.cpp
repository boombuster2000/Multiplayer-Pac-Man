#include "game/utils/highscore_utils.h"
#include "game/components/board.h"
#include "game/components/profile.h"
#include <algorithm>

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

HighscoreVec GetSortedHighscores(const HighscoreMap& highscores)
{
    // Convert unordered_map to vector of pairs
    HighscoreVec sortedHighscores(highscores.begin(), highscores.end());

    // Sort the vector based on scores in descending order
    std::ranges::sort(sortedHighscores, [](const auto& a, const auto& b) { return a.second > b.second; });

    return sortedHighscores;
}

} // namespace highscore_utils

} // namespace game