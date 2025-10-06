#pragma once

#include <stdint.h>

#include "Core/Layer.h"
#include "raylib.h"
#include <vector>
#include "Components/Menu.h"

class MainMenuLayer : public Core::Layer
{

private:
	Components::Menu m_menu;

public:
	MainMenuLayer();
	virtual ~MainMenuLayer();
	

	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
};