#include "MenuLayer.h"
#include "Components/Enums.h"
#include "Components/TextStyle.h"


MenuLayer::MenuLayer()
	: m_menu({ (float)GetScreenWidth()/2, (float)GetScreenHeight()/2 }, Components::AnchorPoint::MIDDLE, Components::Alignment::CENTER, true, 10)
{

	Components::TextStyle unselectedStyle = { 50, BLACK };
	Components::TextStyle selectedStyle = { 50, RED };
	
	m_menu.AddOption(Components::MenuOption("Start Game", selectedStyle, unselectedStyle, true, true));
	m_menu.AddOption(Components::MenuOption("Options", selectedStyle, unselectedStyle, true, false));
	m_menu.AddOption(Components::MenuOption("Exit", selectedStyle, unselectedStyle, true, false));
	m_menu.UpdateOptionsAnchorPointPositions();
}



MenuLayer::~MenuLayer()
{

}

void MenuLayer::OnUpdate(float ts)
{
	if (IsKeyPressed(KEY_DOWN))
	{
		m_menu.SelectNext();
	}
	else if (IsKeyPressed(KEY_UP))
	{
		m_menu.SelectPrevious();
	}

}





void MenuLayer::OnRender()
{
	ClearBackground(WHITE);

	m_menu.Render();
}