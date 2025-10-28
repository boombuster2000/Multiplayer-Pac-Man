#pragma once

#include <string>
#include "raylib.h"
#include "Enums.h"
#include "RenderableObject.h"
#include "TextStyle.h"
#include "DataTypes/Vector2Ex.h"

namespace UIComponents
{

    class Text : public RenderableObject
    {

    private:
        std::string m_text;
        TextStyle m_style;

    public:
        Text(std::string text, TextStyle style, Vector2Ex<int> anchorPointPosition, AnchorPoint AnchorPoint, bool visible);
        ~Text() = default;

        void SetText(std::string text);
        std::string GetText() const;

        void SetFontSize(int fontSize);
        int GetFontSize() const;

        void SetColor(Color color);
        Color GetColor() const;

        void SetStyle(TextStyle style);
        TextStyle GetStyle() const;

        virtual Vector2Ex<int> GetDimensions() const override;
        virtual void Render(Vector2Ex<int> offset = {0, 0}) const override;
    };

}