#include "game/game_application.h"
#include "game/layers/overlay_layer.h"
#include "game/layers/profile_selection_menu_layer.h"

void LoadAllTextures()
{
    game::GameApplication::GetTexturesManager()->AddTexture("wall", "./resources/textures/wall.png");
    game::GameApplication::GetTexturesManager()->AddTexture("path", "./resources/textures/path.png");
    game::GameApplication::GetTexturesManager()->AddTexture("pac-man", "./resources/textures/pac-man.png");
}

int main()
{
    engine::ApplicationSpecification appSpec;
    appSpec.name = "Pac-Man";
    appSpec.width = 1280;
    appSpec.height = 720;
    appSpec.targetFPS = 120;
    appSpec.programExitKey = KEY_NULL;

    game::GameApplication application(appSpec);
    LoadAllTextures();

    game::GameApplication::QueuePush(std::make_unique<OverlayLayer>());
    game::GameApplication::QueuePush(std::make_unique<ProfileSelectionMenuLayer>());
    application.Run();
}