#include "game/file_paths.h"
#include "game/game_application.h"
#include "game/layers/overlay_layer.h"
#include "game/layers/profile_selection_menu_layer.h"

void LoadAllTextures()
{
    auto& texturesManager = game::GameApplication::GetTexturesManager();

    texturesManager.AddTexture("wall", FilePaths::s_texturesDirectory / "wall.png");
    texturesManager.AddTexture("path", FilePaths::s_texturesDirectory / "path.png");
    texturesManager.AddTexture("pac-man", FilePaths::s_texturesDirectory / "pac-man.png");
    texturesManager.AddTexture("blinky", FilePaths::s_texturesDirectory / "blinky.png");
    texturesManager.AddTexture("pinky", FilePaths::s_texturesDirectory / "pinky.png");
    texturesManager.AddTexture("inky", FilePaths::s_texturesDirectory / "inky.png");
    texturesManager.AddTexture("clyde", FilePaths::s_texturesDirectory / "clyde.png");
}

void LoadKeyBinds()
{
    auto& inputManager = game::GameApplication::GetInputManager();

    // Keyboard
    inputManager.AddKeyboardAction("move_up", KEY_W);
    inputManager.AddKeyboardAction("move_up", KEY_UP);
    inputManager.AddKeyboardAction("move_down", KEY_S);
    inputManager.AddKeyboardAction("move_down", KEY_DOWN);
    inputManager.AddKeyboardAction("move_left", KEY_A);
    inputManager.AddKeyboardAction("move_left", KEY_LEFT);
    inputManager.AddKeyboardAction("move_right", KEY_D);
    inputManager.AddKeyboardAction("move_right", KEY_RIGHT);
    inputManager.AddKeyboardAction("confirm", KEY_ENTER);
    inputManager.AddKeyboardAction("pause", KEY_ESCAPE);

    // Gamepad
    inputManager.AddGamepadButtonAction("move_up", GAMEPAD_BUTTON_LEFT_FACE_UP, 0);
    inputManager.AddGamepadButtonAction("move_down", GAMEPAD_BUTTON_LEFT_FACE_DOWN, 0);
    inputManager.AddGamepadButtonAction("move_left", GAMEPAD_BUTTON_LEFT_FACE_LEFT, 0);
    inputManager.AddGamepadButtonAction("move_right", GAMEPAD_BUTTON_LEFT_FACE_RIGHT, 0);
    inputManager.AddGamepadButtonAction("confirm", GAMEPAD_BUTTON_RIGHT_FACE_DOWN, 0);
    inputManager.AddGamepadButtonAction("pause", GAMEPAD_BUTTON_MIDDLE_LEFT, 0);

    inputManager.AddGamepadAxisAction("move_up", GAMEPAD_AXIS_LEFT_Y, 0, false);
    inputManager.AddGamepadAxisAction("move_down", GAMEPAD_AXIS_LEFT_Y, 0, true);
    inputManager.AddGamepadAxisAction("move_left", GAMEPAD_AXIS_LEFT_X, 0, false);
    inputManager.AddGamepadAxisAction("move_right", GAMEPAD_AXIS_LEFT_X, 0, true);
}

int main()
{
    engine::ApplicationSpecification appSpec;
    appSpec.name = "Pac-Man";
    appSpec.width = 1280;
    appSpec.height = 720;
    appSpec.targetFPS = 0;
    appSpec.programExitKey = KEY_NULL;

    game::GameApplication application(appSpec);
    LoadAllTextures();
    LoadKeyBinds();

    game::GameApplication::QueuePush(std::make_unique<OverlayLayer>());
    game::GameApplication::QueuePush(std::make_unique<ProfileSelectionMenuLayer>());
    application.Run();
}