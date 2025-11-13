#pragma once

#include "Text.h"
#include "TextStyle.h"
#include "Enums.h"
#include "RenderableObject.h"
#include <string>
#include <functional>

namespace UIComponents
{

    class MenuOption : public Text
    {

    private:
        bool m_isSelected;
        TextStyle m_selectedStyle;
        TextStyle m_unselectedStyle;
        std::function<void()> m_callback;

    public:
        MenuOption(std::string text,
                   TextStyle selectedStyle,
                   TextStyle unselectedStyle,
                   bool visible,
                   bool isSelected,
                   std::function<void()> callback = nullptr);

        ~MenuOption() = default;

        void SetSelected(bool selected);
        bool IsSelected() const;
        void Select();
    };

}