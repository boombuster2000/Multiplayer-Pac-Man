#pragma once

#include "raylib.h"

class PlayerInput
{
  public:
    virtual bool IsUpPressed() const = 0;
    virtual bool IsDownPressed() const = 0;
    virtual bool IsLeftPressed() const = 0;
    virtual bool IsRightPressed() const = 0;
};

class KeyboardPlayerInput : public PlayerInput
{
  private:
    KeyboardKey m_up;
    KeyboardKey m_down;
    KeyboardKey m_left;
    KeyboardKey m_right;

  public:
    KeyboardPlayerInput(KeyboardKey up, KeyboardKey down, KeyboardKey left, KeyboardKey right);

    bool IsUpPressed() const final;
    bool IsDownPressed() const final;
    bool IsLeftPressed() const final;
    bool IsRightPressed() const final;
};

class GamepadPlayerInput : public PlayerInput
{
  private:
    int m_controllerID;
    GamepadButton m_up;
    GamepadButton m_down;
    GamepadButton m_left;
    GamepadButton m_right;

  public:
    GamepadPlayerInput(int controllerID, GamepadButton up, GamepadButton down, GamepadButton left, GamepadButton right);

    bool IsUpPressed() const final;
    bool IsDownPressed() const final;
    bool IsLeftPressed() const final;
    bool IsRightPressed() const final;
};