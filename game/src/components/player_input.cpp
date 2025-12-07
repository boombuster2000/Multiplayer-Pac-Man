#include "game/components/player_input.h"

KeyboardPlayerInput::KeyboardPlayerInput(KeyboardKey up, KeyboardKey down, KeyboardKey left, KeyboardKey right) :
    m_up(up),
    m_down(down),
    m_left(left),
    m_right(right)
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

GamepadPlayerInput::GamepadPlayerInput(int controllerID,
                                       GamepadButton up,
                                       GamepadButton down,
                                       GamepadButton left,
                                       GamepadButton right) :
    m_controllerID(controllerID),
    m_up(up),
    m_down(down),
    m_left(left),
    m_right(right)
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
