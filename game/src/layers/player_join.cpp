#include "game/layers/player_join.h"

#include "engine/ui/text_menu_option.h"
#include "engine/ui/text_style.h"
#include "game/components/client.h"
#include "game/components/player.h"
#include "game/file_paths.h"
#include "game/game_application.h"
#include "game/layers/create_profile.h"
#include "game/layers/game.h"
#include "game/layers/main_menu.h"
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

    float spacing = fontSize * 1.2f;
    if (screenDivisions >= 4)
    {
        spacing = fontSize;
    }

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

    switch (m_joiningPlayers.size())
    {
    case 2:
        joiningPlayer.pacman.SetColor(Color(67, 255, 65, 255));
        break;
    case 3:
        joiningPlayer.pacman.SetColor(Color(255, 65, 235, 255));
    case 4:
        joiningPlayer.pacman.SetColor(Color(110, 123, 145, 255));
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
        auto newInput =
            std::make_unique<KeyboardPlayerInput>(KEY_W, KEY_S, KEY_A, KEY_D, KEY_LEFT_SHIFT, KEY_LEFT_CONTROL);
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
        auto newInput = std::make_unique<KeyboardPlayerInput>(KEY_UP,
                                                              KEY_DOWN,
                                                              KEY_LEFT,
                                                              KEY_RIGHT,
                                                              KEY_RIGHT_SHIFT,
                                                              KEY_RIGHT_CONTROL);
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
                                                                 GAMEPAD_BUTTON_RIGHT_FACE_DOWN,
                                                                 GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
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
    for (size_t i = 0; i < m_joiningPlayers.size(); ++i)
    {
        auto& player = m_joiningPlayers[i];

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

        if (player.inputControls->IsBackPressed())
        {
            if (i == 0) // Player 1
            {
                TransistionTo(std::make_unique<MainMenuLayer>());
                return; // Exit to avoid using invalidated 'this'
            }

            if (player.state == ReadyState::READY)
            {
                player.profile = nullptr;
                player.state = ReadyState::SELECTING_PROFILE;
                RebuildPlayerMenus();
            }
            else if (player.state == ReadyState::SELECTING_PROFILE)
            {
                m_joiningPlayers.erase(m_joiningPlayers.begin() + i);
                RebuildPlayerMenus();
                --i; // Decrement i to account for the removed element
            }
        }
    }

    // STARTING GAME
    bool startGame = IsKeyPressed(KEY_ENTER);
    if (!startGame)
    {
        for (int controllerID = 0; controllerID < 4; controllerID++)
        {
            if (IsGamepadAvailable(controllerID) && IsGamepadButtonPressed(controllerID, GAMEPAD_BUTTON_MIDDLE_RIGHT))
            {
                startGame = true;
                break;
            }
        }
    }

    if (startGame)
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

                    const int backFontSize = 15;
                    const int y_spacing = backFontSize + 5;
                    int y_start_offset = 10;

                    if (screenDivisions == 4)
                    {
                        const char* back_text = "";
                        if (i == 0)
                        {
                            back_text = "CTRL/B to return to menu";
                        }
                        else
                        {
                            back_text = "CTRL/B to change profile";
                        }
                        const int text_width = MeasureText(back_text, backFontSize);
                        DrawText(back_text,
                                 rect.x + rect.width / 2 - text_width / 2,
                                 rect.y + rect.height - backFontSize - y_start_offset,
                                 backFontSize,
                                 LIGHTGRAY);
                    }
                    else
                    {
                        const char* kb_part1 = "CTRL";
                        const char* kb_part2 =
                            (i == 0) ? " to return to menu (Keyboard)" : " to change profile (Keyboard)";
                        const char* gp_part1 = "B";
                        const char* gp_part2 =
                            (i == 0) ? " to return to menu (Gamepad)" : " to change profile (Gamepad)";

                        const int kb_part1_width = MeasureText(kb_part1, backFontSize);
                        const int kb_total_width = kb_part1_width + MeasureText(kb_part2, backFontSize);
                        const int gp_part1_width = MeasureText(gp_part1, backFontSize);
                        const int gp_total_width = gp_part1_width + MeasureText(gp_part2, backFontSize);

                        float kb_current_X = rect.x + rect.width / 2 - kb_total_width / 2;
                        float gp_current_X = rect.x + rect.width / 2 - gp_total_width / 2;
                        const int y = rect.y + rect.height - y_spacing * 2 - y_start_offset;

                        DrawText(kb_part1, kb_current_X, y, backFontSize, ORANGE);
                        kb_current_X += kb_part1_width;
                        DrawText(kb_part2, kb_current_X, y, backFontSize, LIGHTGRAY);

                        DrawText(gp_part1, gp_current_X, y + y_spacing, backFontSize, ORANGE);
                        gp_current_X += gp_part1_width;
                        DrawText(gp_part2, gp_current_X, y + y_spacing, backFontSize, LIGHTGRAY);
                    }
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
                    const int selectFontSize = 15;
                    const char* sel_kb_part1 = "SHIFT";
                    const char* sel_kb_part2 = " to select (Keyboard)";
                    const char* sel_gp_part1 = "A";
                    const char* sel_gp_part2 = " to select (Gamepad)";

                    const int sel_kb_part1_width = MeasureText(sel_kb_part1, selectFontSize);
                    const int sel_kb_total_width = sel_kb_part1_width + MeasureText(sel_kb_part2, selectFontSize);
                    const int sel_gp_part1_width = MeasureText(sel_gp_part1, selectFontSize);
                    const int sel_gp_total_width = sel_gp_part1_width + MeasureText(sel_gp_part2, selectFontSize);

                    float sel_kb_current_X = rect.x + rect.width / 2 - sel_kb_total_width / 2;
                    float sel_gp_current_X = rect.x + rect.width / 2 - sel_gp_total_width / 2;
                    const int y_spacing = selectFontSize + 5;
                    const int y = rect.y + rect.height - y_spacing * 4 - 10;

                    DrawText(sel_kb_part1, sel_kb_current_X, y, selectFontSize, ORANGE);
                    sel_kb_current_X += sel_kb_part1_width;
                    DrawText(sel_kb_part2, sel_kb_current_X, y, selectFontSize, LIGHTGRAY);

                    DrawText(sel_gp_part1, sel_gp_current_X, y + y_spacing, selectFontSize, ORANGE);
                    sel_gp_current_X += sel_gp_part1_width;
                    DrawText(sel_gp_part2, sel_gp_current_X, y + y_spacing, selectFontSize, LIGHTGRAY);

                    const int backFontSize = 15;
                    const char* backPart1 = "CTRL";
                    const char* backPart2 = " to leave (Keyboard)";
                    const char* backPart3 = "B";
                    const char* backPart4 = " to leave (Gamepad)";

                    const int backPart1Width = MeasureText(backPart1, backFontSize);
                    const int backPart2Width = MeasureText(backPart2, backFontSize);
                    const int backPart3Width = MeasureText(backPart3, backFontSize);
                    const int backPart4Width = MeasureText(backPart4, backFontSize);
                    const int backTotalWidth = backPart1Width + backPart2Width;
                    const int backTotalWidth2 = backPart3Width + backPart4Width;

                    float backCurrentX = rect.x + rect.width / 2 - backTotalWidth / 2;
                    float backCurrentX2 = rect.x + rect.width / 2 - backTotalWidth2 / 2;
                    const int back_y_spacing = backFontSize + 5;
                    const int backY = y + y_spacing * 2;

                    DrawText(backPart1, backCurrentX, backY, backFontSize, ORANGE);
                    backCurrentX += backPart1Width;
                    DrawText(backPart2, backCurrentX, backY, backFontSize, LIGHTGRAY);

                    DrawText(backPart3, backCurrentX2, backY + back_y_spacing, backFontSize, ORANGE);
                    backCurrentX2 += backPart3Width;
                    DrawText(backPart4, backCurrentX2, backY + back_y_spacing, backFontSize, LIGHTGRAY);
                }
                break;
            }
        }
    }

    // Check if all players are ready and display "PRESS ENTER / START TO START"
    if (joiningPlayersCount > 0)
    {
        bool allPlayersReady = true;
        for (const auto& player : m_joiningPlayers)
        {
            if (player.state != ReadyState::READY)
            {
                allPlayersReady = false;
                break;
            }
        }

        if (allPlayersReady)
        {
            const int startFontSize = 40;
            const char* part1 = "PRESS ";
            const char* part2 = "ENTER";
            const char* part3 = " / ";
            const char* part4 = "START";
            const char* part5 = " TO START";

            const int part1Width = MeasureText(part1, startFontSize);
            const int part2Width = MeasureText(part2, startFontSize);
            const int part3Width = MeasureText(part3, startFontSize);
            const int part4Width = MeasureText(part4, startFontSize);
            const int part5Width = MeasureText(part5, startFontSize);
            const int totalWidth = part1Width + part2Width + part3Width + part4Width + part5Width;

            const int x = GetScreenWidth() / 2 - totalWidth / 2;
            const int y = GetScreenHeight() - startFontSize - 100;

            // Draw a black background behind the text
            DrawRectangle(x - 10, y - 5, totalWidth + 20, startFontSize + 10, WHITE);

            float currentX = x;
            DrawText(part1, currentX, y, startFontSize, DARKGRAY);
            currentX += part1Width;
            DrawText(part2, currentX, y, startFontSize, ORANGE);
            currentX += part2Width;
            DrawText(part3, currentX, y, startFontSize, DARKGRAY);
            currentX += part3Width;
            DrawText(part4, currentX, y, startFontSize, ORANGE);
            currentX += part4Width;
            DrawText(part5, currentX, y, startFontSize, DARKGRAY);
        }
    }
}
