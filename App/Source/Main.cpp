#include "Core/Application.h"

#include "MainMenuComponents/MainMenuLayer.h"
#include "GameComponents/GameLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Pac-Man";
	appSpec.Width = 1280;
	appSpec.Height = 720;

	Core::Application application(appSpec);

	application.PushLayer<GameLayer>();
	application.Run();
}