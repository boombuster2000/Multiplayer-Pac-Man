#include "Core/Application.h"

#include "MainMenuComponents/MainMenuLayer.h"
#include "GameComponents/GameLayer.h"

void LoadAllTextures()
{
	Core::Application::GetTexturesManager()->AddTexture("wall", "./Resources/Textures/Wall.png");
    Core::Application::GetTexturesManager()->AddTexture("path", "./Resources/Textures/Path.png");
    Core::Application::GetTexturesManager()->AddTexture("pac-man", "./Resources/Textures/Pac-Man.png");
}

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Pac-Man";
	appSpec.Width = 1280;
	appSpec.Height = 720;

	Core::Application application(appSpec);
	LoadAllTextures();

	application.PushLayer<GameLayer>();
	application.Run();
}