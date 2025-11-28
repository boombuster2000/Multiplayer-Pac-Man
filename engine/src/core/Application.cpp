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

Application::Application(const ApplicationSpecification& specification) : m_specification(specification)
{
    assert(!s_application && "Only one Application instance may exist at a time");
    s_application = this;

    InitWindow(specification.width, specification.height, specification.name.c_str());
    SetExitKey(specification.programExitKey);

    if (specification.targetFPS > 0)
        SetTargetFPS(specification.targetFPS);
}

Application::~Application()
{
    assert(s_application == this && "Destroying unexpected Application instance");
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
    s_pendingActions.emplace(MakePushAction(std::move(layer)));
}

void Application::QueuePop(std::type_index layerType)
{
    s_pendingActions.emplace(MakePopAction(layerType));
}

void Application::QueueTransition(std::type_index fromType, std::unique_ptr<Layer> toLayer)
{
    s_pendingActions.emplace(MakeTransitionAction(fromType, std::move(toLayer)));
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
            auto it = FindLayerIt(action.fromType);

            if (it != m_layerStack.end())
                m_layerStack.erase(it);
        }

        break;

        case LayerActionType::TRANSITION: {
            auto it = FindLayerIt(action.fromType);

            if (it != m_layerStack.end())
                *it = std::move(action.toLayer);
        }

        break;
        }
    }
}

std::vector<std::unique_ptr<Layer>>::iterator Application::FindLayerIt(std::type_index desiredType)
{
    // why: lambda variable name must not shadow function argument
    return std::ranges::find_if(m_layerStack, [&](const std::unique_ptr<Layer>& layer) {
        return std::type_index(typeid(*layer)) == desiredType;
    });
}

LayerAction Application::MakePushAction(std::unique_ptr<Layer> layer)
{
    return LayerAction{LayerActionType::PUSH, std::type_index(typeid(void)), std::move(layer)};
}

LayerAction Application::MakePopAction(std::type_index layerType)
{
    return LayerAction{LayerActionType::POP, layerType, nullptr};
}

LayerAction Application::MakeTransitionAction(std::type_index fromType, std::unique_ptr<Layer> toLayer)
{
    return LayerAction{LayerActionType::TRANSITION, fromType, std::move(toLayer)};
}

Application& Application::Get()
{
    assert(s_application);
    return *s_application;
}
} // namespace engine