#pragma once

#include "engine/ui/enums.h"
#include "engine/ui/renderable_object.h"
#include "engine/ui/text.h"
#include "engine/ui/text_style.h"
#include <functional>
#include <string>

namespace ui
{

class MenuOption : public Text
{

  private:
    bool m_isSelected;
    TextStyle m_selectedStyle;
    TextStyle m_unselectedStyle;
    std::function<void()> m_callback;

  public:
    MenuOption(std::string text, TextStyle selectedStyle, TextStyle unselectedStyle, bool visible, bool IsSelected,
               std::function<void()> callback = nullptr);

    ~MenuOption() = default;

    void SetSelected(bool selected);
    bool IsSelected() const;
    void Select();
};

} // namespace ui