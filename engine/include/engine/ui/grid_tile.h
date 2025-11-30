#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/renderable_object.h"
#include "raylib.h"
#include <nlohmann/json.hpp>
using nlohmann::json;

namespace ui
{
class GridTile : public RenderableObject
{
  private:
    Vector2Ex<float> m_dimensions;

  public:
    GridTile();
    GridTile(Vector2Ex<float> dimensions, Vector2Ex<float> position);

    Vector2Ex<float> GetDimensions() const override;

    void Render(Vector2Ex<float> offset = {0, 0}) const override;

    friend void to_json(json&, const GridTile&);
    friend void from_json(const json&, GridTile&);
};
} // namespace ui
