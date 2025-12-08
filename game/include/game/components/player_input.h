#pragma once

#include "raylib.h"

class PlayerInput
{
  public:
    virtual ~PlayerInput() = default;
    virtual bool IsUpPressed() const = 0;
    virtual bool IsDownPressed() const = 0;
    virtual bool IsLeftPressed() const = 0;
    virtual bool IsRightPressed() const = 0;
    virtual bool IsActionPressed() const = 0;
    virtual bool IsBackPressed() const = 0;
    virtual bool IsEqual(const PlayerInput* other) const = 0;
};

class KeyboardPlayerInput : public PlayerInput
{
  private:
    KeyboardKey m_up;
    KeyboardKey m_down;
    KeyboardKey m_left;
    KeyboardKey m_right;
    KeyboardKey m_action;
    KeyboardKey m_back;

  public:
    KeyboardPlayerInput(KeyboardKey up, KeyboardKey down, KeyboardKey left, KeyboardKey right, KeyboardKey action,
                        KeyboardKey back);

    bool IsUpPressed() const final;
    bool IsDownPressed() const final;
    bool IsLeftPressed() const final;
    bool IsRightPressed() const final;
    bool IsActionPressed() const final;
    bool IsBackPressed() const final;
    bool IsEqual(const PlayerInput* other) const final;
};

class GamepadPlayerInput : public PlayerInput
{
  private:
    int m_controllerID;
    GamepadButton m_up;
    GamepadButton m_down;
    GamepadButton m_left;
    GamepadButton m_right;
    GamepadButton m_action;
    GamepadButton m_back;

  public:
    GamepadPlayerInput(int controllerID, GamepadButton up, GamepadButton down, GamepadButton left, GamepadButton right,
                       GamepadButton action, GamepadButton back);

    bool IsUpPressed() const final;
    bool IsDownPressed() const final;
    bool IsLeftPressed() const final;
    bool IsRightPressed() const final;
    bool IsActionPressed() const final;
    bool IsBackPressed() const final;
    bool IsEqual(const PlayerInput* other) const final;
};