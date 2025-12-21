#pragma once

#include "base_menu.h"
#include "engine/ui/text.h"

class ProfileSelectionMenuLayer : public BaseMenuLayer
{
  private:
    ui::TextStyle m_titleStyle = {50, GRAY};

    ui::Text m_title{
        "Select Profile",
        m_titleStyle,
        Vector2Ex<float>{static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 8)},
        ui::AnchorPoint::TOP_MIDDLE,
        true};

  private:
    void SetupMenuOptions() final;

  public:
    ProfileSelectionMenuLayer();

    void OnRender() override;
};