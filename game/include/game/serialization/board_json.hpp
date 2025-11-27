#pragma once
#include "engine/serialization/grid_json.hpp"
#include "engine/serialization/json_helpers.hpp"
#include "game/components/board.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

inline void to_json(json& j, const Board& board)
{
    const auto& grid = static_cast<const ui::Grid<Tile>&>(board);
    to_json(j, grid);

    j["name"] = board.m_name;
    j["highScores"] = board.m_highScores;
}

inline void from_json(const json& j, Board& board)
{
    auto& grid = static_cast<ui::Grid<Tile>&>(board);
    from_json(j, grid);

    serialization::get_required_field(j, "name", board.m_name, "Board", 500);
    serialization::get_required_field(j, "highScores", board.m_highScores, "Board", 501);
}
