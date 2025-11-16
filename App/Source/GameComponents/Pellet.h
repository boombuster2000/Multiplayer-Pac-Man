#pragma once

#include "Datatypes/Vector2Ex.h"
#include "UIComponents/RenderableObject.h"
#include "raylib.h"

class Pellet : public UIComponents::RenderableObject
{
  public:
    enum class Type : int
    {
        // Point values
        NORMAL,
        SUPER,
        NONE
    };

    struct TypePointValues
    {
        int normal;
        int super;
        int none;
    };

  private:
    Type m_type;
    float radius;

    static inline TypePointValues s_typePoints{
        10, // Normal
        50, // Super
        0   // None
    };

  public:
    Pellet();
    Pellet(Vector2Ex<float> position, Type type);

    Type GetType() const;
    void SetType(const Type &type);

    int GetValue() const;

    static int GetTypePoints(const Type &t);
    static void SetTypePoints(const TypePointValues &newValues);

    virtual Vector2Ex<float> GetDimensions() const override;
    virtual void Render(Vector2Ex<float> offset = {0, 0}) const override;
};