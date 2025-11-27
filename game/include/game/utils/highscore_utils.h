#pragma once

#include "engine/core/transparent_string_hasher.h"
#include <string>
#include <unordered_map>

class Board;
class Profile;

namespace game
{
namespace highscore_utils
{
using HighscoreMap = std::unordered_map<std::string, int, TransparentStringHash, std::equal_to<>>;

void SaveHighscore(::Board& board, ::Profile& profile, const int score);

HighscoreMap SortHighscores(const HighscoreMap& highscores);

} // namespace highscore_utils

} // namespace game