#pragma once
#include "DataTypes/Vector2Ex.h"
#include "Pellet.h"
#include "UIComponents/GridTile.h"
#include "raylib.h"
#include <memory>

class Tile : public UIComponents::GridTile
{

  public:
    enum class Type : int
    {
        Wall,
        Path,
        None,
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

    Pellet &GetPellet();
    const Pellet &GetPellet() const;

    void SetPosition(const Vector2Ex<float> &position);

    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
};