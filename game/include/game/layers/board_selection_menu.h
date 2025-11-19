#pragma once

#include "base_menu.h"
#include "game/components/player.h"

class BoardSelectionMenuLayer : public BaseMenuLayer
{
  private:
    virtual void SetupMenuOptions() override;

  public:
    BoardSelectionMenuLayer();
    virtual ~BoardSelectionMenuLayer() = default;
};