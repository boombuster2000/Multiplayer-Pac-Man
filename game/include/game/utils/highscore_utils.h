#pragma once

#include "engine/core/transparent_string_hasher.h"
#include <string>
#include <unordered_map>
#include <vector>

class Board;
class Profile;

namespace game::highscore_utils
{

using HighscoreMap = std::unordered_map<std::string, int, TransparentStringHash, std::equal_to<>>;
using HighscoreVec = std::vector<std::pair<std::string, int>>;

void SaveHighscore(::Board& board, ::Profile& profile, const int score);

HighscoreVec GetSortedHighscores(const HighscoreMap& highscores);

} // namespace game::highscore_utils