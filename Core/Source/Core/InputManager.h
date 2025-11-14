#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Core
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
  public:
    InputManager();
    ~InputManager() = default;

    void Update();

    bool IsAction(const std::string &action, InputState state) const;
    float GetActionValue(const std::string &action) const;

    void AddKeyboardAction(const std::string &action, int keyboardKey);
    void AddGamepadButtonAction(const std::string &action, int gamepadButton, int gamepadIndex = 0);
    void AddGamepadAxisAction(const std::string &action, int gamepadAxis, int gamepadIndex = 0, bool positive = true);

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

    std::unordered_map<std::string, std::vector<Action>> m_actions;
    std::unordered_map<std::string, InputState> m_actionStates;
    std::unordered_map<std::string, float> m_actionValues;
};
} // namespace Core
