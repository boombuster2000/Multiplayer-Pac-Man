#pragma once

#include <stdint.h>

#include "Core/Layer.h"
#include "raylib.h"
#include <vector>
#include "Components/Menu.h"

class MenuLayer : public Core::Layer
{

private:
	Components::Menu m_menu;

public:
	MenuLayer();
	virtual ~MenuLayer();
	

	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
};