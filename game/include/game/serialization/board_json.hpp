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

    j["pacman_speed"] = board.m_pacmanSpeed;
    j["ghost-speed"] = board.m_ghostSpeed;

    j["speed-increment"] = board.m_speedIncreaseIncrement;
    j["ghost-release-time-decrement"] = board.m_ghostReleaseTimeDecrease;

    j["blinkySpawnPointIndex"] = board.m_blinkySpawnPointIndex;
    j["blinkyGuardPointIndex"] = board.m_blinkyGuardPointIndex;
    j["blinkyReleaseTime"] = board.m_blinkyReleaseTime;

    j["pinkySpawnPointIndex"] = board.m_pinkySpawnPointIndex;
    j["pinkyGuardPointIndex"] = board.m_pinkyGuardPointIndex;
    j["pinkyReleaseTime"] = board.m_pinkyReleaseTime;

    j["inkySpawnPointIndex"] = board.m_inkySpawnPointIndex;
    j["inkyGuardPointIndex"] = board.m_inkyGuardPointIndex;
    j["inkyReleaseTime"] = board.m_inkyReleaseTime;

    j["clydeSpawnPointIndex"] = board.m_clydeSpawnPointIndex;
    j["clydeGuardPointIndex"] = board.m_clydeGuardPointIndex;
    j["clydeReleaseTime"] = board.m_clydeReleaseTime;

    j["player1SpawnPointIndex"] = board.m_player1SpawnPointIndex;
    j["player2SpawnPointIndex"] = board.m_player2SpawnPointIndex;
    j["player3SpawnPointIndex"] = board.m_player3SpawnPointIndex;
    j["player4SpawnPointIndex"] = board.m_player4SpawnPointIndex;

    j["frightenedTimeDuration"] = board.m_frightenedTimeDuration;
    j["ghostModeTime"] = board.m_ghostModeTime;
}

inline void from_json(const json& j, Board& board)
{
    auto& grid = static_cast<ui::Grid<Tile>&>(board);
    from_json(j, grid);

    serialization::get_required_field(j, "name", board.m_name, "Board");
    serialization::get_required_field(j, "highScores", board.m_highScores, "Board");

    serialization::get_required_field(j, "pacman_speed", board.m_pacmanSpeed, "Board");
    serialization::get_required_field(j, "ghost-speed", board.m_ghostSpeed, "Board");

    serialization::get_required_field(j, "speed-increment", board.m_speedIncreaseIncrement, "Board");
    serialization::get_required_field(j, "ghost-release-time-decrement", board.m_ghostReleaseTimeDecrease, "Board");

    serialization::get_required_field(j, "blinkySpawnPointIndex", board.m_blinkySpawnPointIndex, "Board");
    serialization::get_required_field(j, "blinkyGuardPointIndex", board.m_blinkyGuardPointIndex, "Board");
    serialization::get_required_field(j, "blinkyReleaseTime", board.m_blinkyReleaseTime, "Board");

    serialization::get_required_field(j, "pinkySpawnPointIndex", board.m_pinkySpawnPointIndex, "Board");
    serialization::get_required_field(j, "pinkyGuardPointIndex", board.m_pinkyGuardPointIndex, "Board");
    serialization::get_required_field(j, "pinkyReleaseTime", board.m_pinkyReleaseTime, "Board");

    serialization::get_required_field(j, "inkySpawnPointIndex", board.m_inkySpawnPointIndex, "Board");
    serialization::get_required_field(j, "inkyGuardPointIndex", board.m_inkyGuardPointIndex, "Board");
    serialization::get_required_field(j, "inkyReleaseTime", board.m_inkyReleaseTime, "Board");

    serialization::get_required_field(j, "clydeSpawnPointIndex", board.m_clydeSpawnPointIndex, "Board");
    serialization::get_required_field(j, "clydeGuardPointIndex", board.m_clydeGuardPointIndex, "Board");
    serialization::get_required_field(j, "clydeReleaseTime", board.m_clydeReleaseTime, "Board");

    serialization::get_required_field(j, "player1SpawnPointIndex", board.m_player1SpawnPointIndex, "Board");
    serialization::get_required_field(j, "player2SpawnPointIndex", board.m_player2SpawnPointIndex, "Board");
    serialization::get_required_field(j, "player3SpawnPointIndex", board.m_player3SpawnPointIndex, "Board");
    serialization::get_required_field(j, "player4SpawnPointIndex", board.m_player4SpawnPointIndex, "Board");

    serialization::get_required_field(j, "frightenedTimeDuration", board.m_frightenedTimeDuration, "Board");
    serialization::get_required_field(j, "ghostModeTime", board.m_ghostModeTime, "Board");
}
