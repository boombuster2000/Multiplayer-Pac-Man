#include "game/components/pellet.h"
#include "game/game_application.h"

Pellet::Pellet() : RenderableObject(), m_type(Type::NONE)
{
}

Pellet::Pellet(Vector2Ex<float> position, const Type type)
    : RenderableObject(position, ui::AnchorPoint::MIDDLE), m_type(type)
{
}

Pellet::Type Pellet::GetType() const
{
    return m_type;
}

void Pellet::SetType(const Type& type)
{
    m_type = type;
}

int Pellet::GetValue() const
{
    return Pellet::GetTypePoints(m_type);
}

int Pellet::GetTypePoints(const Type& t)
{
    switch (t)
    {
    case Type::NORMAL:
        return s_typePoints.normal;
    case Type::SUPER:
        return s_typePoints.super;
    case Type::NONE:
        return s_typePoints.none;
    }

    throw std::runtime_error("Invalid Type");
}

void Pellet::SetTypePoints(const TypePointValues& newValues)
{
    s_typePoints = newValues;
}

Vector2Ex<float> Pellet::GetDimensions() const
{
    return Vector2Ex<float>(radius, radius);
}

void Pellet::Render(Vector2Ex<float> offset) const
{
    if (m_type == Type::NONE)
        return;

    DrawCircleV(GetWorldOrigin(), radius, WHITE);
}
