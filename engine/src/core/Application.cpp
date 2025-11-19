#include "engine/core/application.h"
#include "engine/core/input_manager.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <raylib.h>

namespace engine
{

static Application* s_application = nullptr;
static std::shared_ptr<TexturesManager> s_texturesManager = nullptr;
static std::shared_ptr<InputManager> s_inputManager = nullptr;
std::queue<LayerAction> Application::s_pendingActions;

Application::Application(const ApplicationSpecification& specification) : m_specification(specification)
{
    s_application = this;
    s_texturesManager = std::make_shared<TexturesManager>();
    s_inputManager = std::make_shared<InputManager>();

    // Keyboard
    s_inputManager->AddKeyboardAction("move_up", KEY_W);
    s_inputManager->AddKeyboardAction("move_up", KEY_UP);
    s_inputManager->AddKeyboardAction("move_down", KEY_S);
    s_inputManager->AddKeyboardAction("move_down", KEY_DOWN);
    s_inputManager->AddKeyboardAction("move_left", KEY_A);
    s_inputManager->AddKeyboardAction("move_left", KEY_LEFT);
    s_inputManager->AddKeyboardAction("move_right", KEY_D);
    s_inputManager->AddKeyboardAction("move_right", KEY_RIGHT);
    s_inputManager->AddKeyboardAction("confirm", KEY_ENTER);
    s_inputManager->AddKeyboardAction("pause", KEY_ESCAPE);

    // Gamepad
    s_inputManager->AddGamepadButtonAction("move_up", GAMEPAD_BUTTON_LEFT_FACE_UP, 0);
    s_inputManager->AddGamepadButtonAction("move_down", GAMEPAD_BUTTON_LEFT_FACE_DOWN, 0);
    s_inputManager->AddGamepadButtonAction("move_left", GAMEPAD_BUTTON_LEFT_FACE_LEFT, 0);
    s_inputManager->AddGamepadButtonAction("move_right", GAMEPAD_BUTTON_LEFT_FACE_RIGHT, 0);
    s_inputManager->AddGamepadButtonAction("confirm", GAMEPAD_BUTTON_RIGHT_FACE_DOWN, 0);
    s_inputManager->AddGamepadButtonAction("pause", GAMEPAD_BUTTON_MIDDLE_LEFT, 0);

    s_inputManager->AddGamepadAxisAction("move_up", GAMEPAD_AXIS_LEFT_Y, 0, false);
    s_inputManager->AddGamepadAxisAction("move_down", GAMEPAD_AXIS_LEFT_Y, 0, true);
    s_inputManager->AddGamepadAxisAction("move_left", GAMEPAD_AXIS_LEFT_X, 0, false);
    s_inputManager->AddGamepadAxisAction("move_right", GAMEPAD_AXIS_LEFT_X, 0, true);

    InitWindow(specification.width, specification.height, specification.name.c_str());
    SetExitKey(specification.programExitKey);

    if (specification.targetFPS > 0)
        SetTargetFPS(specification.targetFPS);
}

Application::~Application()
{
    CloseWindow();
    s_application = nullptr;
}

std::shared_ptr<TexturesManager> Application::GetTexturesManager()
{
    return s_texturesManager;
}

std::shared_ptr<InputManager> Application::GetInputManager()
{
    return s_inputManager;
}

void Application::Run()
{
    m_running = true;

    float lastTime = GetTime();

    // Main Application loop
    while (m_running)
    {
        if (WindowShouldClose())
        {
            Stop();
            break;
        }

        s_inputManager->Update();

        ProcessPendingActions();

        // Main layer update here
        for (const std::unique_ptr<Layer>& layer : m_layerStack)
        {
            if (layer->GetState() == Layer::State::ACTIVE)
                layer->OnUpdate(GetFrameTime());
        }

        BeginDrawing();

        // NOTE: rendering can be done elsewhere (eg. render thread)
        for (const std::unique_ptr<Layer>& layer : m_layerStack)
        {
            if (layer->GetState() != Layer::State::SUSPENDED_UPDATE_AND_RENDER)
                layer->OnRender();
        }

        EndDrawing();
    }
}

void Application::Stop()
{
    m_running = false;
}

void Application::QueuePush(std::unique_ptr<Layer> layer)
{
    s_pendingActions.push({LayerActionType::PUSH, std::type_index(typeid(void)), std::move(layer)});
}

void Application::QueuePop(std::type_index layerType)
{
    s_pendingActions.push({LayerActionType::POP, layerType, nullptr});
}

void Application::QueueTransition(std::type_index fromType, std::unique_ptr<Layer> toLayer)
{
    s_pendingActions.push({LayerActionType::TRANSITION, fromType, std::move(toLayer)});
}

void Application::ProcessPendingActions()
{
    while (!s_pendingActions.empty())
    {
        LayerAction action = std::move(s_pendingActions.front());
        s_pendingActions.pop();

        switch (action.type)
        {
        case LayerActionType::PUSH:
            m_layerStack.push_back(std::move(action.toLayer));
            break;

        case LayerActionType::POP: {
            auto it = std::find_if(m_layerStack.begin(), m_layerStack.end(), [&](const std::unique_ptr<Layer>& layer) {
                return std::type_index(typeid(*layer)) == action.fromType;
            });
            if (it != m_layerStack.end())
                m_layerStack.erase(it);
        }
        break;

        case LayerActionType::TRANSITION: {
            auto it = std::find_if(m_layerStack.begin(), m_layerStack.end(), [&](const std::unique_ptr<Layer>& layer) {
                return std::type_index(typeid(*layer)) == action.fromType;
            });
            if (it != m_layerStack.end())
                *it = std::move(action.toLayer);
        }
        break;
        }
    }
}

Application& Application::Get()
{
    assert(s_application);
    return *s_application;
}

} // namespace engine