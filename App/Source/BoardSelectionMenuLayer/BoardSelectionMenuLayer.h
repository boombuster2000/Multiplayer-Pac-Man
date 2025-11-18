#pragma once

#include "BaseMenuLayer/BaseMenuLayer.h"

class BoardSelectionMenuLayer : public BaseMenuLayer
{
  private:
    virtual void SetupMenuOptions() override;

  public:
    BoardSelectionMenuLayer();
    virtual ~BoardSelectionMenuLayer() = default;
};