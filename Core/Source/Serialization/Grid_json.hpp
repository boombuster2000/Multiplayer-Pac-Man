#pragma once
#include <nlohmann/json.hpp>
#include "UIComponents/Grid.h"
#include "RenderableObject_json.hpp"
#include "GridTile_json.hpp"
#include "Vector2Ex_json.hpp"

using nlohmann::json;

namespace UIComponents {

    // Template serialization for Grid<T>
    template <typename T>
    inline void to_json(json &j, const Grid<T> &grid)
    {
        static_assert(std::is_base_of<GridTile, T>::value,
                    "T must derive from UIComponents::GridTile");

        json base;
        to_json(base, static_cast<const RenderableObject &>(grid));

        json gridArray = json::array();
        for (const auto &row : grid)
        {
            json jsonRow = json::array();
            for (const auto &tile : row)
            {
                json tile_j;
                to_json(tile_j, tile);
                jsonRow.push_back(tile_j);
            }
            gridArray.push_back(jsonRow);
        }

        j = base;
        j["tileDimensions"] = grid.GetTileDimensions();
        j["spacing"] = grid.GetSpacing();
        j["gridSize"] = grid.GetGridSize();
        j["tiles"] = gridArray;
    }

    template <typename T>
    inline void from_json(const json &j, Grid<T> &grid)
    {
        static_assert(std::is_base_of<GridTile, T>::value,
                    "T must derive from UIComponents::GridTile");
        
        from_json(j, static_cast<RenderableObject &>(grid));

        auto gridSize = j.at("gridSize").get<Vector2Ex<size_t>>();
        auto tileDim = j.at("tileDimensions").get<Vector2Ex<float>>();
        auto spacing = j.at("spacing").get<Vector2Ex<float>>();

        grid.SetGridSize(gridSize);
        grid.SetTileDimensions(tileDim);
        grid.SetSpacing(spacing);

        const auto &tiles = j.at("tiles");
        for (size_t y = 0; y < tiles.size(); ++y)
        {
            const auto &row = tiles[y];
            for (size_t x = 0; x < row.size(); ++x)
            {
                grid.Set(y, x, row[x].template get<T>());
            }
        }
    }

}
