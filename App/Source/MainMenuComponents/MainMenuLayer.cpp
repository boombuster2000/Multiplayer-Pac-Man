#include "MainMenuLayer.h"
#include "UIComponents/Enums.h"
#include "UIComponents/TextStyle.h"
#include "Core/Application.h"
#include <iostream>

MainMenuLayer::MainMenuLayer()
	: m_menu({(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2}, UIComponents::AnchorPoint::MIDDLE, UIComponents::Alignment::CENTER, true, 10)
{
	using namespace UIComponents;
	TextStyle unselectedStyle = {50, BLACK};
	TextStyle selectedStyle = {60, RED};

	m_menu.AddOption(MenuOption("Start Game", selectedStyle, unselectedStyle, true, true, []() { std::cout << "Start Game" << std::endl; }));
	m_menu.AddOption(MenuOption("Options", selectedStyle, unselectedStyle, true, false, []() { std::cout << "Options" << std::endl; }));
	m_menu.AddOption(MenuOption("Exit", selectedStyle, unselectedStyle, true, false, []() { Core::Application::Get().Stop(); }));
}

MainMenuLayer::~MainMenuLayer()
{
}

void MainMenuLayer::OnUpdate(float ts)
{
	if (IsKeyPressed(KEY_DOWN))
	{
		m_menu.SelectNext();
	}
	else if (IsKeyPressed(KEY_UP))
	{
		m_menu.SelectPrevious();
	}
	else if (IsKeyPressed(KEY_ENTER))
	{
		m_menu.ConfirmSelection();
	}

	if (m_menu.IsUIUpdateNeeded())
	{
		m_menu.UpdateOptionsAnchorPointPositions();
	}
}

void MainMenuLayer::OnRender()
{
	ClearBackground(WHITE);

	m_menu.Render();
}