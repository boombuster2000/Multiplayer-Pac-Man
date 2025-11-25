#pragma once
#include "engine/core/vector2ex.h"
#include "engine/ui/menu_option.h"
#include "engine/ui/renderable_object.h"
#include <memory>
#include <vector>

namespace ui
{

class Menu : public RenderableObject
{

  private:
    std::vector<std::unique_ptr<MenuOption>> m_options;

    int m_selectedIndex;
    float m_spacing;
    Alignment m_alignment;
    AnchorPoint m_anchorPoint;
    bool m_isUIupdateNeeded;

  public:
    Menu(Vector2Ex<float> anchorPointPosition, AnchorPoint AnchorPoint, Alignment alignment, bool visible,
         float spacing);

    void AddOption(std::unique_ptr<MenuOption> option);
    void DeleteOption(int index);
    void ClearOptions();

    void SelectNext();
    void SelectPrevious();
    void ConfirmSelection();

    const MenuOption& GetSelectedOption() const;
    int GetSelectedIndex() const;

    bool IsUIUpdateNeeded() const;
    void UpdateOptionsAnchorPointPositions();
    Vector2Ex<float> GetDimensions() const override;
    void Render(Vector2Ex<float> offset = {0, 0}) const override;
};

} // namespace ui