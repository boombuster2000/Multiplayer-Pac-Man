#pragma once

#include "Text.h"
#include "TextStyle.h"
#include "Enums.h"
#include "RenderableObject.h"
#include <string>


namespace UIComponents {

    
	class MenuOption : public Text
	{

    private:
        bool m_isSelected;
        TextStyle m_selectedStyle;
        TextStyle m_unselectedStyle;
    
	public:
        
        MenuOption(std::string text,
            TextStyle selectedStyle,
            TextStyle unselectedStyle,
            bool visible,
            bool isSelected
        );

		~MenuOption() = default;

        void SetSelected(bool selected);
        bool IsSelected() const;
	};

}