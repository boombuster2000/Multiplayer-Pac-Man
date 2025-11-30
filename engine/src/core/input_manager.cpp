#include "engine/core/input_manager.h"
#include "raylib.h"

namespace engine
{

void InputManager::Update()
{
    for (auto const& [name, actions] : m_actions)
    {
        auto [isDown, value] = GetActionStatus(actions);
        UpdateActionState(name, isDown);
        m_actionValues[name] = value;
    }
}

std::pair<bool, float> InputManager::GetActionStatus(const std::vector<Action>& actions)
{
    bool isDown = false;
    float value = 0.0f;

    for (const auto& action : actions)
    {
        ProcessKeyboard(action, isDown, value);
        ProcessGamepadButton(action, isDown, value);
        ProcessGamepadAxis(action, isDown, value);
    }

    return {isDown, value};
}

void InputManager::ProcessKeyboard(const Action& action, bool& isDown, float& value)
{
    if (action.keyboardKey != -1)
    {
        if (IsKeyDown(action.keyboardKey))
        {
            isDown = true;
            value = 1.0f;
        }
    }
}

void InputManager::ProcessGamepadButton(const Action& action, bool& isDown, float& value)
{
    if (action.gamepadButton != -1)
    {
        if (IsGamepadButtonDown(action.gamepadIndex, action.gamepadButton))
        {
            isDown = true;
            value = 1.0f;
        }
    }
}

void InputManager::ProcessGamepadAxis(const Action& action, bool& isDown, float& value)
{
    if (action.gamepadAxis != -1)
    {
        float axisValue = GetGamepadAxisMovement(action.gamepadIndex, action.gamepadAxis);
        if (action.positive)
        {
            if (axisValue > 0.5f)
            {
                isDown = true;
                value = axisValue;
            }
        }
        else
        {
            if (axisValue < -0.5f)
            {
                isDown = true;
                value = -axisValue;
            }
        }
    }
}

void InputManager::UpdateActionState(const std::string& name, bool isDown)
{
    using enum InputState;
    InputState& state = m_actionStates.try_emplace(name, UP).first->second;

    if (isDown)
    {
        if (state == UP || state == RELEASED)
        {
            state = PRESSED;
        }
        else
        {
            state = DOWN;
        }
    }
    else
    {
        if (state == DOWN || state == PRESSED)
        {
            state = RELEASED;
        }
        else
        {
            state = UP;
        }
    }
}

bool InputManager::IsAction(std::string_view action, InputState state) const
{
    if (auto it = m_actionStates.find(action); it != m_actionStates.end())
    {
        return it->second == state;
    }
    return false;
}

float InputManager::GetActionValue(std::string_view action) const
{
    if (auto it = m_actionValues.find(action); it != m_actionValues.end())
    {
        return it->second;
    }
    return 0.0f;
}

void InputManager::AddKeyboardAction(const std::string& action, int keyboardKey)
{
    m_actions[action].emplace_back(action, keyboardKey);
}

void InputManager::AddGamepadButtonAction(const std::string& action, int gamepadButton, int gamepadIndex)
{
    m_actions[action].emplace_back(action, -1, gamepadButton, -1, gamepadIndex);
}

void InputManager::AddGamepadAxisAction(const std::string& action, int gamepadAxis, int gamepadIndex, bool positive)
{
    m_actions[action].emplace_back(action, -1, -1, gamepadAxis, gamepadIndex, positive);
}
} // namespace engine
