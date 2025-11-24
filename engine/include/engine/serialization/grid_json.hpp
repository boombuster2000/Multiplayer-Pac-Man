#pragma once
#include "engine/serialization/json_converters.hpp"
#include "engine/ui/grid.h"
#include "raylib.h"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

namespace ui
{

// Template serialization for Grid<T>
template <typename T>
inline void to_json(json& j, const Grid<T>& grid)
{
    static_assert(std::is_base_of<GridTile, T>::value, "T must derive from ui::GridTile");

    // Serialize base RenderableObject
    to_json(j, static_cast<const RenderableObject&>(grid));

    j["tile_dimensions"] = grid.m_tileDimensions;
    j["spacing"] = grid.m_spacing;

    json gridArray = json::array();
    for (const auto& row : grid)
    {
        json jsonRow = json::array();
        for (const auto& tile : row)
        {
            json tile_j;
            to_json(tile_j, tile);
            jsonRow.push_back(tile_j);
        }
        gridArray.push_back(jsonRow);
    }

    j["grid"] = gridArray;
}

template <typename T>
inline void from_json(const json& j, Grid<T>& grid)
{
    if (!j.is_object())
        throw std::runtime_error("Failed to deserialize Grid: JSON is not an object.");

    static_assert(std::is_base_of<GridTile, T>::value, "T must derive from ui::GridTile");
    from_json(j, static_cast<RenderableObject&>(grid));

    try
    {
        j.at("tile_dimensions").get_to(grid.m_tileDimensions);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Grid.tile_dimensions: " + std::string(e.what()));
    }

    try
    {
        j.at("spacing").get_to(grid.m_spacing);
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Grid.spacing: " + std::string(e.what()));
    }

    try
    {
        const auto& grid_j = j.at("grid");
        if (!grid_j.is_array())
            throw json::type_error::create(302, "grid must be an array of arrays", &j);

        size_t y_size = grid_j.size();
        size_t x_size = (y_size > 0) ? grid_j.at(0).size() : 0;

        grid.m_gridSize = {x_size, y_size};
        grid.m_grid.assign(y_size, std::vector<T>(x_size));

        for (size_t y = 0; y < y_size; ++y)
        {
            const auto& row_j = grid_j.at(y);
            if (!row_j.is_array())
                throw json::type_error::create(302, "grid must be an array of arrays", &j);

            for (size_t x = 0; x < x_size; ++x)
            {
                if (x < row_j.size())
                {
                    try
                    {
                        row_j.at(x).get_to(grid.m_grid[y][x]);
                    }
                    catch (const std::exception& e)
                    {
                        throw std::runtime_error("Failed to deserialize element at Grid.grid[" + std::to_string(y) +
                                                 "][" + std::to_string(x) + "]: " + e.what());
                    }
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to deserialize Grid.grid: " + std::string(e.what()));
    }
}
} // namespace ui