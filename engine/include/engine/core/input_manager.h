#pragma once

#include "transparent_string_hasher.h"
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

        // Keyboard Constructor
        Action(const std::string& actionName, int key) : name(actionName), keyboardKey(key)
        {
        }

        // Gamepad Button Constructor
        Action(const std::string& actionName, int unused1, int button, int unused2, int index)
            : name(actionName), gamepadButton(button), gamepadIndex(index)
        {
        }

        // Gamepad Axis Constructor
        Action(const std::string& actionName, int unused1, int unused2, int axis, int index, bool polarity)
            : name(actionName), gamepadAxis(axis), gamepadIndex(index), positive(polarity)
        {
        }
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

    bool IsAction(std::string_view action, InputState state) const;
    float GetActionValue(std::string_view action) const;

    void AddKeyboardAction(const std::string& action, int keyboardKey);
    void AddGamepadButtonAction(const std::string& action, int gamepadButton, int gamepadIndex = 0);
    void AddGamepadAxisAction(const std::string& action, int gamepadAxis, int gamepadIndex = 0, bool positive = true);
};
} // namespace engine
