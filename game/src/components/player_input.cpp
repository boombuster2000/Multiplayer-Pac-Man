#include "game/components/player_input.h"

KeyboardPlayerInput::KeyboardPlayerInput(KeyboardKey up, KeyboardKey down, KeyboardKey left, KeyboardKey right,
                                       KeyboardKey action, KeyboardKey back) :
    m_up(up),
    m_down(down),
    m_left(left),
    m_right(right),
    m_action(action),
    m_back(back)
{
}

bool KeyboardPlayerInput::IsUpPressed() const
{
    return IsKeyPressed(m_up);
}

bool KeyboardPlayerInput::IsDownPressed() const
{
    return IsKeyPressed(m_down);
}

bool KeyboardPlayerInput::IsLeftPressed() const
{
    return IsKeyPressed(m_left);
}

bool KeyboardPlayerInput::IsRightPressed() const
{
    return IsKeyPressed(m_right);
}

bool KeyboardPlayerInput::IsActionPressed() const
{
    return IsKeyPressed(m_action);
}

bool KeyboardPlayerInput::IsBackPressed() const
{
    return IsKeyPressed(m_back);
}

bool KeyboardPlayerInput::IsEqual(const PlayerInput* other) const
{
    if (const auto* otherKeyboard = dynamic_cast<const KeyboardPlayerInput*>(other))
    {
        return m_up == otherKeyboard->m_up && m_down == otherKeyboard->m_down && m_left == otherKeyboard->m_left &&
               m_right == otherKeyboard->m_right && m_action == otherKeyboard->m_action &&
               m_back == otherKeyboard->m_back;
    }
    return false;
}

GamepadPlayerInput::GamepadPlayerInput(int controllerID,
                                       GamepadButton up,
                                       GamepadButton down,
                                       GamepadButton left,
                                       GamepadButton right,
                                       GamepadButton action,
                                       GamepadButton back) :
    m_controllerID(controllerID),
    m_up(up),
    m_down(down),
    m_left(left),
    m_right(right),
    m_action(action),
    m_back(back)
{
}

bool GamepadPlayerInput::IsUpPressed() const
{
    return IsGamepadButtonPressed(m_controllerID, m_up);
}

bool GamepadPlayerInput::IsDownPressed() const
{
    return IsGamepadButtonPressed(m_controllerID, m_down);
}

bool GamepadPlayerInput::IsLeftPressed() const
{
    return IsGamepadButtonPressed(m_controllerID, m_left);
}

bool GamepadPlayerInput::IsRightPressed() const
{
    return IsGamepadButtonPressed(m_controllerID, m_right);
}

bool GamepadPlayerInput::IsActionPressed() const
{
    return IsGamepadButtonPressed(m_controllerID, m_action);
}

bool GamepadPlayerInput::IsBackPressed() const
{
    return IsGamepadButtonPressed(m_controllerID, m_back);
}

bool GamepadPlayerInput::IsEqual(const PlayerInput* other) const
{
    if (const auto* otherGamepad = dynamic_cast<const GamepadPlayerInput*>(other))
    {
        return m_controllerID == otherGamepad->m_controllerID;
    }
    return false;
}
