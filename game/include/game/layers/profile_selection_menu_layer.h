#pragma once

#include "base_menu.h"
#include "main_menu.h"

class ProfileSelectionMenuLayer : public BaseMenuLayer
{
  private:
    void SetupMenuOptions() final;

  public:
    ProfileSelectionMenuLayer();
};