#include "Core/Application.h"

#include "MainMenuLayer/MainMenuLayer.h"

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
	appSpec.targetFPS = 120;
	appSpec.programExitKey = KEY_NULL;

	Core::Application application(appSpec);
	LoadAllTextures();

	Core::Application::QueuePush<MainMenuLayer>();
	application.Run();
}