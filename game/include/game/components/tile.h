#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/grid_tile.h"
#include "game/components/pellet.h"
#include "raylib.h"
#include <memory>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Tile : public ui::GridTile
{

  public:
    enum class Type : int
    {
        WALL,
        PATH,
        NONE,
    };

  private:
    Type m_type;
    Pellet m_pellet;

  public:
    Tile();
    Tile(Type type, Vector2Ex<float> postion, Vector2Ex<float> dimensions);
    Tile(Type type, Pellet::Type pelletType, Vector2Ex<float> postion, Vector2Ex<float> dimensions);

    Type GetType() const;
    void SetType(Type type);

    Pellet& GetPellet();
    const Pellet& GetPellet() const;

    void SetPosition(const Vector2Ex<float>& position);

    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;

    friend void to_json(json&, const Tile&);
    friend void from_json(const json&, Tile&);
};