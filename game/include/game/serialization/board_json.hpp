#pragma once
#include "engine/serialization/grid_json.hpp"
#include "game/components/board.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

    if (!j.contains("name"))
        throw std::runtime_error("Failed to deserialize Board: missing field 'name'");

    j.at("name").get_to(board.m_name);

    if (!j.contains("highScores"))
        throw std::runtime_error("Failed to deserialize Board: missing field 'highScores'");

    j.at("highScores").get_to(board.m_highScores);
}
