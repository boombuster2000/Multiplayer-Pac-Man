#pragma once

#include "base_menu.h"
#include "engine/ui/text_box_option.h"
#include <memory>

class CreateProfileLayer : public BaseMenuLayer
{
  private:
    ui::TextBoxOption* m_profileNameInput;

  private:
    virtual void SetupMenuOptions() override;

    void OnTextBoxClicked();
    void OnContinueClicked();
    void OnBackClicked();

  public:
    CreateProfileLayer();
    virtual ~CreateProfileLayer() = default;

    virtual void OnUpdate(float ts) override;

    void OnRender();
};