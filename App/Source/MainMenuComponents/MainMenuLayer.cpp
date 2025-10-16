#include "MainMenuLayer.h"
#include "UIComponents/Enums.h"
#include "UIComponents/TextStyle.h"

MainMenuLayer::MainMenuLayer()
	: m_menu({GetScreenWidth() / 2, GetScreenHeight() / 2}, UIComponents::AnchorPoint::MIDDLE, UIComponents::Alignment::CENTER, true, 10)
{

	UIComponents::TextStyle unselectedStyle = {50, BLACK};
	UIComponents::TextStyle selectedStyle = {60, RED};

	m_menu.AddOption(UIComponents::MenuOption("Start Game", selectedStyle, unselectedStyle, true, true));
	m_menu.AddOption(UIComponents::MenuOption("Options", selectedStyle, unselectedStyle, true, false));
	m_menu.AddOption(UIComponents::MenuOption("Exit", selectedStyle, unselectedStyle, true, false));
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