#pragma once

#include "transparentStringHasher.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace engine
{
enum class InputState
{
    PRESSED,
    DOWN,
    RELEASED,
    UP
};

class InputManager
{

  private:
    struct Action
    {
        std::string_view name;
        int keyboardKey = -1;
        int gamepadButton = -1;
        int gamepadAxis = -1;
        int gamepadIndex = 0;
        bool positive = true;
    };

    std::unordered_map<std::string_view, std::vector<Action>, TransparentStringHash, std::equal_to<>> m_actions;
    std::unordered_map<std::string_view, InputState, TransparentStringHash, std::equal_to<>> m_actionStates;
    std::unordered_map<std::string_view, float, TransparentStringHash, std::equal_to<>> m_actionValues;

    void ProcessKeyboard(const Action& action, bool& isDown, float& value);
    void ProcessGamepadButton(const Action& action, bool& isDown, float& value);
    void ProcessGamepadAxis(const Action& action, bool& isDown, float& value);
    std::pair<bool, float> GetActionStatus(const std::vector<Action>& actions);
    void UpdateActionState(std::string_view name, bool isDown);

  public:
    InputManager() = default;
    ~InputManager() = default;

    void Update();

    bool IsAction(std::string_view action, InputState state) const;
    float GetActionValue(std::string_view action) const;

    void AddKeyboardAction(std::string_view action, int keyboardKey);
    void AddGamepadButtonAction(std::string_view action, int gamepadButton, int gamepadIndex = 0);
    void AddGamepadAxisAction(std::string_view action, int gamepadAxis, int gamepadIndex = 0, bool positive = true);
};
} // namespace engine
