#include "game/layers/player_join.h"

#include "engine/ui/text_menu_option.h"
#include "engine/ui/text_style.h"
#include "game/components/client.h"
#include "game/components/player.h"
#include "game/file_paths.h"
#include "game/game_application.h"
#include "game/layers/create_profile.h"
#include "game/layers/game.h"
#include "game/serialization/profile_json.hpp"
#include "game/utils/file_utils.h"
#include "nlohmann/json.hpp"
#include "raylib.h"
#include <format>
#include <memory>
#include <set>
#include <string>
#include <vector>

void PlayerJoinLayer::RebuildPlayerMenus()
{
    const int joiningPlayersCount = m_joiningPlayers.size();
    const int screenDivisions = GetScreenDivisions(joiningPlayersCount);

    float fontSize = 30.f;
    if (screenDivisions >= 4)
    {
        fontSize = 15.f;
    }
    else if (screenDivisions == 2)
    {
        fontSize = 20.f;
    }
    const float spacing = fontSize * 1.2f;

    ui::TextStyle unselectedStyle;
    unselectedStyle.fontSize = fontSize;
    unselectedStyle.color = DARKGRAY;

    ui::TextStyle selectedStyle;
    selectedStyle.fontSize = fontSize;
    selectedStyle.color = ORANGE;

    std::set<std::string> takenUsernames;
    for (const auto& p : m_joiningPlayers)
    {
        if (p.profile && !p.profile->GetUsername().empty())
        {
            takenUsernames.insert(std::string(p.profile->GetUsername()));
        }
    }

    const auto profileJsons = game::file_utils::ReadJsonsFromDirectory(FilePaths::s_profilesDirectory);

    for (auto& player : m_joiningPlayers)
    {
        if (player.state == ReadyState::SELECTING_PROFILE)
        {
            player.profileSelectionMenu.SetSpacing(spacing);
            player.profileSelectionMenu.ClearOptions();

            for (const auto& profileJson : profileJsons)
            {
                auto profile = std::make_shared<Profile>(profileJson.get<Profile>());

                if (takenUsernames.find(std::string(profile->GetUsername())) == takenUsernames.end())
                {
                    auto option = std::make_unique<ui::TextMenuOption>(std::string(profile->GetUsername()),
                                                                       selectedStyle,
                                                                       unselectedStyle,
                                                                       false,
                                                                       [this, &player, profile]() {
                                                                           player.profile = profile;
                                                                           player.state = ReadyState::READY;
                                                                           RebuildPlayerMenus();
                                                                       });
                    player.profileSelectionMenu.AddOption(std::move(option));
                }
            }

            auto createOption = std::make_unique<ui::TextMenuOption>(
                "Create New Profile",
                selectedStyle,
                unselectedStyle,
                false,
                [this, &player]() {
                    static int newPlayerId = 1;
                    std::string newUsername = "NewPlayer" + std::to_string(newPlayerId++);

                    // A more robust solution would check for username uniqueness on disk too

                    player.profile = std::make_shared<Profile>(newUsername);
                    player.profile->Save();
                    player.state = ReadyState::READY;
                    RebuildPlayerMenus();
                });
            player.profileSelectionMenu.AddOption(std::move(createOption));
        }
    }
}

void PlayerJoinLayer::AddJoiningPlayer(PlayerInput* controls)
{
    m_joiningPlayers.emplace_back(controls);
    auto& joiningPlayer = m_joiningPlayers.back();

    if (!m_isFirstPlayerJoined)
    {
        joiningPlayer.profile = game::GameApplication::Get().GetProfile();
        joiningPlayer.state = ReadyState::READY;
        m_isFirstPlayerJoined = true;
        return;
    }

    RebuildPlayerMenus();
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
    m_joiningPlayers.reserve(4);
}

void PlayerJoinLayer::OnUpdate(float ts)
{
    // Check for new players joining
    // KEYBOARD CONTROLS
    if (IsKeyPressed(KEY_W))
    {
        auto newInput = std::make_unique<KeyboardPlayerInput>(KEY_W, KEY_S, KEY_A, KEY_D, KEY_LEFT_SHIFT);
        bool alreadyExists = false;
        for (const auto& player : m_joiningPlayers)
        {
            if (player.inputControls && player.inputControls->IsEqual(newInput.get()))
            {
                alreadyExists = true;
                break;
            }
        }

        if (!alreadyExists)
        {
            AddJoiningPlayer(newInput.release());
        }
    }

    if (IsKeyPressed(KEY_UP))
    {
        auto newInput = std::make_unique<KeyboardPlayerInput>(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RIGHT_SHIFT);
        bool alreadyExists = false;
        for (const auto& player : m_joiningPlayers)
        {
            if (player.inputControls && player.inputControls->IsEqual(newInput.get()))
            {
                alreadyExists = true;
                break;
            }
        }

        if (!alreadyExists)
        {
            AddJoiningPlayer(newInput.release());
        }
    }

    // GAMEPAD CONTROLS
    const int numberOfControllers = 4;

    for (int controllerID = 0; controllerID < numberOfControllers; controllerID++)
    {
        if (IsGamepadAvailable(controllerID) && IsGamepadButtonPressed(controllerID, GAMEPAD_BUTTON_MIDDLE_LEFT))
        {
            auto newInput = std::make_unique<GamepadPlayerInput>(controllerID,
                                                                 GAMEPAD_BUTTON_LEFT_FACE_UP,
                                                                 GAMEPAD_BUTTON_LEFT_FACE_DOWN,
                                                                 GAMEPAD_BUTTON_LEFT_FACE_LEFT,
                                                                 GAMEPAD_BUTTON_LEFT_FACE_RIGHT,
                                                                 GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
            bool alreadyExists = false;
            for (const auto& player : m_joiningPlayers)
            {
                if (player.inputControls && player.inputControls->IsEqual(newInput.get()))
                {
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists)
            {
                AddJoiningPlayer(newInput.release());
            }
        }
    }

    // Update existing joining players
    for (auto& player : m_joiningPlayers)
    {
        if (player.state == ReadyState::SELECTING_PROFILE)
        {
            if (player.inputControls->IsUpPressed())
            {
                player.profileSelectionMenu.SelectPrevious();
            }
            if (player.inputControls->IsDownPressed())
            {
                player.profileSelectionMenu.SelectNext();
            }
            if (player.inputControls->IsActionPressed())
            {
                player.profileSelectionMenu.ConfirmSelection();
            }
        }
    }

    // STARTING GAME
    if (IsKeyPressed(KEY_ENTER))
    {
        std::vector<Client> clients;

        for (auto& joiningPlayer : m_joiningPlayers)
        {
            if (joiningPlayer.state != ReadyState::READY)
                return; // Do not start if not all players are ready

            Client client = {joiningPlayer.profile, Player(), joiningPlayer.pacman, joiningPlayer.inputControls};

            clients.push_back(client);
        }

        if (!clients.empty())
        {
            if (m_boardPath == "built-in")
                TransistionTo(std::make_unique<GameLayer>(clients));
            else
                TransistionTo(std::make_unique<GameLayer>(clients, m_boardPath));
        }
    }
}

void PlayerJoinLayer::OnRender()
{
    const int joiningPlayersCount = m_joiningPlayers.size();

    if (joiningPlayersCount == 0)
    {
        ClearBackground(BLACK);
        const int fontSize = 30;

        // Define parts and colors
        const char* part1 = "PRESS THE ";
        const char* part2 = "UP/W";
        const char* part3 = " BUTTON ON KEYBOARD OR ";
        const char* part4 = "SELECT";
        const char* part5 = " ON CONTROLLER TO JOIN";

        // Calculate total width to center the entire message
        const int totalWidth = MeasureText(part1, fontSize) + MeasureText(part2, fontSize) +
                               MeasureText(part3, fontSize) + MeasureText(part4, fontSize) +
                               MeasureText(part5, fontSize);

        float currentX = GetScreenWidth() / 2 - totalWidth / 2;
        float y = GetScreenHeight() / 2 - fontSize / 2;

        DrawText(part1, currentX, y, fontSize, LIGHTGRAY);
        currentX += MeasureText(part1, fontSize);

        DrawText(part2, currentX, y, fontSize, ORANGE); // Highlight UP/W
        currentX += MeasureText(part2, fontSize);

        DrawText(part3, currentX, y, fontSize, LIGHTGRAY);
        currentX += MeasureText(part3, fontSize);

        DrawText(part4, currentX, y, fontSize, ORANGE); // Highlight SELECT
        currentX += MeasureText(part4, fontSize);

        DrawText(part5, currentX, y, fontSize, LIGHTGRAY);

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
            auto& player = m_joiningPlayers[i];
            const int fontSize = 30;
            std::string line1, line2;
            Color color1 = ORANGE;
            Color color2 = DARKGRAY;

            switch (player.state)
            {
            case ReadyState::READY:
                line1 = "Player " + std::to_string(i + 1) + " Ready!";
                if (player.profile)
                {
                    line2 = std::format("Profile: {}", player.profile->GetUsername());
                }

                {
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
                break;
            case ReadyState::SELECTING_PROFILE:
                player.profileSelectionMenu.SetPosition({rect.x + rect.width / 2, rect.y + rect.height / 2});
                if (player.profileSelectionMenu.IsUIUpdateNeeded())
                {
                    player.profileSelectionMenu.UpdateOptionsAnchorPointPositions();
                }
                player.profileSelectionMenu.Render();

                {
                    const char* selectText = "SHIFT to select (Keyboard) / A to select (Gamepad)";
                    const int selectFontSize = 20;
                    const int selectTextWidth = MeasureText(selectText, selectFontSize);
                    DrawText(selectText,
                             rect.x + rect.width / 2 - selectTextWidth / 2,
                             rect.y + rect.height - selectFontSize - 10, // Position it near the bottom of the rect
                             selectFontSize,
                             LIGHTGRAY);
                }
                break;
            }
        }
    }
}
