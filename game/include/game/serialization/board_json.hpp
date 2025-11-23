#pragma once
#include "engine/serialization/grid_json.hpp"
#include "game/components/board.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

inline void to_json(json& j, const Board& board) {
    const auto& grid = static_cast<const ui::Grid<Tile>&>(board);
    to_json(j, grid);
}

inline void from_json(const json& j, Board& board) {
    auto& grid = static_cast<ui::Grid<Tile>&>(board);
    from_json(j, grid);
}
