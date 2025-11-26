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
        std::string name;
        int keyboardKey = -1;
        int gamepadButton = -1;
        int gamepadAxis = -1;
        int gamepadIndex = 0;
        bool positive = true;
    };

    std::unordered_map<std::string, std::vector<Action>, TransparentStringHash, std::equal_to<>> m_actions;
    std::unordered_map<std::string, InputState, TransparentStringHash, std::equal_to<>> m_actionStates;
    std::unordered_map<std::string, float, TransparentStringHash, std::equal_to<>> m_actionValues;

    void ProcessKeyboard(const Action& action, bool& isDown, float& value);
    void ProcessGamepadButton(const Action& action, bool& isDown, float& value);
    void ProcessGamepadAxis(const Action& action, bool& isDown, float& value);
    std::pair<bool, float> GetActionStatus(const std::vector<Action>& actions);
    void UpdateActionState(const std::string& name, bool isDown);

  public:
    InputManager() = default;
    ~InputManager() = default;

    void Update();

    bool IsAction(const std::string& action, InputState state) const;
    float GetActionValue(const std::string& action) const;

    void AddKeyboardAction(const std::string& action, int keyboardKey);
    void AddGamepadButtonAction(const std::string& action, int gamepadButton, int gamepadIndex = 0);
    void AddGamepadAxisAction(const std::string& action, int gamepadAxis, int gamepadIndex = 0, bool positive = true);
};
} // namespace engine
