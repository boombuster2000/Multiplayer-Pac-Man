#pragma once
#include <memory>
#include "raylib.h"
#include "UIComponents/GridTile.h"
#include "DataTypes/Vector2Ex.h"

class Tile : public UIComponents::GridTile
{

public:
    enum class Type : int
    {
        Wall,
        Path,
    };

    enum class PelletType : int
    {
        Pellet,
        PowerPellet,
        None
    };

private:
    Type m_type;
    PelletType m_pelletType;

public:
    Tile(Type type, Vector2Ex<int> postion, Vector2Ex<int> dimensions);

    Type GetType() const;
    void SetType(Type type);

    virtual void Render(Vector2Ex<int> offset = {0, 0}) const override;
};