#include "engine/core/application.h"
#include "engine/core/input_manager.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <raylib.h>

namespace engine
{

Application* Application::s_application = nullptr;
std::queue<LayerAction> Application::s_pendingActions;

Application::Application(const ApplicationSpecification& specification)
    : m_specification(specification), m_texturesManager(), m_inputManager()
{
    s_application = this;

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

TexturesManager& Application::GetTexturesManager()
{
    return Get().m_texturesManager;
}

InputManager& Application::GetInputManager()
{
    return Get().m_inputManager;
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

        m_inputManager.Update();

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
    s_pendingActions.emplace(LayerActionType::PUSH, std::type_index(typeid(void)), std::move(layer));
}

void Application::QueuePop(std::type_index layerType)
{
    s_pendingActions.emplace(LayerActionType::POP, layerType, nullptr);
}

void Application::QueueTransition(std::type_index fromType, std::unique_ptr<Layer> toLayer)
{
    s_pendingActions.emplace(LayerActionType::TRANSITION, fromType, std::move(toLayer));
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
            auto it = std::ranges::find_if(m_layerStack, [&](const std::unique_ptr<Layer>& layer) {
                return std::type_index(typeid(*layer)) == action.fromType;
            });

            if (it != m_layerStack.end())
                m_layerStack.erase(it);
        }

        break;

        case LayerActionType::TRANSITION: {
            auto it = std::ranges::find_if(m_layerStack, [&](const std::unique_ptr<Layer>& layer) {
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