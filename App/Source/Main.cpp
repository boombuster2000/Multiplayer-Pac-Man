#include "Core/Application.h"

#include "MenuLayer.h"

int main()
{
	Core::ApplicationSpecification appSpec;
	appSpec.Name = "Pac-Man";
	appSpec.Width = 1280;
	appSpec.Height = 720;

	Core::Application application(appSpec);
	application.PushLayer<MenuLayer>();
	application.Run();
}