#include "game/layers/player_join.h"
#include "game/components/client.h"
#include "game/components/player.h"
#include "game/game_application.h"
#include "game/layers/game.h"
#include "raylib.h"
#include <format>
#include <string>
#include <vector>

void PlayerJoinLayer::AddJoiningPlayer(PlayerInput* controls)
{
    JoiningPlayer joiningPlayer;
    if (!m_isFirstPlayerJoined) // Assigns the already loaded profile to controller device
    {
        joiningPlayer.profile = game::GameApplication::Get().GetProfile();
        joiningPlayer.inputControls = controls;
        joiningPlayer.state = ReadyState::READY;
        m_isFirstPlayerJoined = true;
    }
    else // Need to split screen and allow player to select or create a profile.
    {
        joiningPlayer.inputControls = controls;
        joiningPlayer.state = ReadyState::SELECTING_PROFILE;
    }

    m_joiningPlayers.push_back(joiningPlayer);
}

int PlayerJoinLayer::GetScreenDivisions(const int playerCount) const
{
    if (playerCount == 3)
        return 4;

    return playerCount;
}

PlayerJoinLayer::PlayerJoinLayer(std::string_view boardFilePath) :
    m_boardPath(boardFilePath)
{
}

void PlayerJoinLayer::OnUpdate(float ts)
{

    // KEYBOARD CONTROLS
    if (IsKeyPressed(KEY_W))
    {
        PlayerInput* inputControls = new KeyboardPlayerInput(KEY_W, KEY_S, KEY_A, KEY_D);
        AddJoiningPlayer(inputControls);
    }

    if (IsKeyPressed(KEY_UP))
    {
        PlayerInput* inputControls = new KeyboardPlayerInput(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
        AddJoiningPlayer(inputControls);
    }

    // GAMEPAD CONTROLS
    const int numberOfControllers = 4;

    for (int controllerID = 0; controllerID < numberOfControllers; controllerID++)
    {
        // if (!IsGamepadAvailable(controllerID))
        //     continue;

        if (!IsGamepadButtonPressed(controllerID, GAMEPAD_BUTTON_MIDDLE_LEFT))
            continue;

        PlayerInput* inputControls = new GamepadPlayerInput(controllerID,
                                                            GAMEPAD_BUTTON_LEFT_FACE_UP,
                                                            GAMEPAD_BUTTON_LEFT_FACE_DOWN,
                                                            GAMEPAD_BUTTON_LEFT_FACE_LEFT,
                                                            GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        AddJoiningPlayer(inputControls);
    }

    // STARTING GAME

    if (IsKeyPressed(KEY_ENTER))
    {
        std::vector<Client> clients;

        for (auto& joiningPlayer : m_joiningPlayers)
        {
            Client client = {
                joiningPlayer.profile,
                Player(),
                joiningPlayer.pacman,
                joiningPlayer.inputControls,
            };

            clients.push_back(client);
        }

        if (m_boardPath == "built-in")
            TransistionTo(std::make_unique<GameLayer>(clients));
        else
            TransistionTo(std::make_unique<GameLayer>(clients, m_boardPath));
    }
}

void PlayerJoinLayer::OnRender()
{
    const int joiningPlayersCount = m_joiningPlayers.size();

    if (joiningPlayersCount == 0)
    {
        ClearBackground(BLACK);
        const char* text = "PRESS A BUTTON ON KEYBOARD OR CONTROLLER TO JOIN";
        const int fontSize = 30;
        const int textWidth = MeasureText(text, fontSize);
        DrawText(text, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - fontSize / 2, fontSize, LIGHTGRAY);
        return;
    }

    const int screenDivisions = GetScreenDivisions(joiningPlayersCount);
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    std::vector<Rectangle> screenRects;
    if (screenDivisions <= 1)
    {
        screenRects.push_back({0.f, 0.f, (float)screenWidth, (float)screenHeight});
    }
    else if (screenDivisions == 2)
    {
        screenRects.push_back({0.f, 0.f, (float)screenWidth / 2, (float)screenHeight});
        screenRects.push_back({(float)screenWidth / 2, 0.f, (float)screenWidth / 2, (float)screenHeight});
    }
    else if (screenDivisions == 4)
    {
        screenRects.push_back({0.f, 0.f, (float)screenWidth / 2, (float)screenHeight / 2});
        screenRects.push_back({(float)screenWidth / 2, 0.f, (float)screenWidth / 2, (float)screenHeight / 2});
        screenRects.push_back({0.f, (float)screenHeight / 2, (float)screenWidth / 2, (float)screenHeight / 2});
        screenRects.push_back(
            {(float)screenWidth / 2, (float)screenHeight / 2, (float)screenWidth / 2, (float)screenHeight / 2});
    }

    for (int i = 0; i < screenDivisions; ++i)
    {
        const auto& rect = screenRects[i];
        DrawRectangleLinesEx(rect, 2, LIGHTGRAY);

        if (i < joiningPlayersCount)
        {
            const auto& player = m_joiningPlayers[i];
            const int fontSize = 30;
            std::string line1, line2;
            Color color1 = RAYWHITE;
            Color color2 = LIGHTGRAY;

            switch (player.state)
            {
            case ReadyState::READY:
                line1 = "Player " + std::to_string(i + 1) + " Ready!";
                if (player.profile)
                {
                    line2 = std::format("Profile: {}", player.profile->GetUsername());
                }
                break;
            case ReadyState::SELECTING_PROFILE:
                line1 = "Player " + std::to_string(i + 1);
                line2 = "SELECTING PROFILE";
                break;
            }

            const int textWidth1 = MeasureText(line1.c_str(), fontSize);
            DrawText(line1.c_str(),
                     rect.x + rect.width / 2 - textWidth1 / 2,
                     rect.y + rect.height / 2 - fontSize,
                     fontSize,
                     color1);

            const int textWidth2 = MeasureText(line2.c_str(), fontSize - 10);
            DrawText(line2.c_str(),
                     rect.x + rect.width / 2 - textWidth2 / 2,
                     rect.y + rect.height / 2 + (fontSize - 10),
                     fontSize - 10,
                     color2);
        }
        else
        {
            const char* text = "Waiting for player...";
            const int fontSize = 20;
            const int textWidth = MeasureText(text, fontSize);
            DrawText(text,
                     rect.x + rect.width / 2 - textWidth / 2,
                     rect.y + rect.height / 2 - fontSize / 2,
                     fontSize,
                     DARKGRAY);
        }
    }
}
