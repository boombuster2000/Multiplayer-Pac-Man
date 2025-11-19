#pragma once

#include "base_menu.h"
#include "engine/ui/text_box.h"
#include <memory>

class CreateProfileLayer : public BaseMenuLayer
{
  private:
    std::unique_ptr<ui::TextBox> m_profileNameInput;

    virtual void SetupMenuOptions() override;

    void OnContinueClicked();
    void OnBackClicked();

  public:
    CreateProfileLayer();
    virtual ~CreateProfileLayer() = default;

    virtual void OnUpdate(float ts) override;

    void OnRender();
};