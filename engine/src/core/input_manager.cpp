#include "engine/core/input_manager.h"
#include "raylib.h"

namespace engine
{

void InputManager::Update()
{
    for (auto const& [name, actions] : m_actions)
    {
        bool isDown = false;
        float value = 0.0f;

        for (const auto& action : actions)
        {
            if (action.keyboardKey != -1)
            {
                if (IsKeyDown(action.keyboardKey))
                {
                    isDown = true;
                    value = 1.0f;
                }
            }

            if (action.gamepadButton != -1)
            {
                if (IsGamepadButtonDown(action.gamepadIndex, action.gamepadButton))
                {
                    isDown = true;
                    value = 1.0f;
                }
            }

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

        InputState& state = m_actionStates[name];

        if (isDown)
        {
            if (state == InputState::UP || state == InputState::RELEASED)
            {
                state = InputState::PRESSED;
            }
            else
            {
                state = InputState::DOWN;
            }
        }
        else
        {
            if (state == InputState::DOWN || state == InputState::PRESSED)
            {
                state = InputState::RELEASED;
            }
            else
            {
                state = InputState::UP;
            }
        }

        m_actionValues[name] = value;
    }
}

bool InputManager::IsAction(const std::string& action, InputState state) const
{
    auto it = m_actionStates.find(action);
    if (it != m_actionStates.end())
    {
        return it->second == state;
    }
    return false;
}

float InputManager::GetActionValue(const std::string& action) const
{
    auto it = m_actionValues.find(action);
    if (it != m_actionValues.end())
    {
        return it->second;
    }
    return 0.0f;
}

void InputManager::AddKeyboardAction(const std::string& action, int keyboardKey)
{
    m_actions[action].push_back({action, keyboardKey});
}

void InputManager::AddGamepadButtonAction(const std::string& action, int gamepadButton, int gamepadIndex)
{
    m_actions[action].push_back({action, -1, gamepadButton, -1, gamepadIndex});
}

void InputManager::AddGamepadAxisAction(const std::string& action, int gamepadAxis, int gamepadIndex, bool positive)
{
    m_actions[action].push_back({action, -1, -1, gamepadAxis, gamepadIndex, positive});
}
} // namespace engine
