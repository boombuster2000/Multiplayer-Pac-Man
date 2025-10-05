#pragma once

#include <string>
#include "raylib.h"
#include "Enums.h"
#include "RenderableObject.h"
#include "TextStyle.h"

namespace Components {

	class Text : public RenderableObject
	{

    private:
        std::string m_text;
        TextStyle m_style;

	public:
        
        Text(std::string text, TextStyle style, Vector2 anchorPointPosition, AnchorPoint AnchorPoint, bool visible);
		~Text() = default;

        void SetText(std::string text);
        std::string GetText() const;

        void SetFontSize(int fontSize);
        int GetFontSize() const;
        
        void SetColor(Color color);
        Color GetColor() const;

        void SetStyle(TextStyle style);
        TextStyle GetStyle() const;

        virtual Vector2 GetSize() const override;
        virtual void Render() const override;
        
	};

}