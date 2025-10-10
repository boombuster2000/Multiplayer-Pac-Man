#pragma once
#include "raylib.h"
#include <memory>

class Tile {

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
    Tile(Type type = Type::Path);


    Type GetType() const;
    void SetType(Type type);


    void Render(Vector2 position, Vector2 size) const;
};