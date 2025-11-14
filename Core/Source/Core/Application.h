#pragma once

#include "InputManager.h"
#include "Layer.h"
#include "TexturesManager.h"
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <typeindex>
#include <vector>

namespace Core
{
class InputManager;

struct ApplicationSpecification
{
    std::string Name = "Raylib Application";

    uint32_t Width = 1280;
    uint32_t Height = 720;
    int targetFPS = 60;

    KeyboardKey programExitKey = KEY_ESCAPE;
};

enum class LayerActionType
{
    Push,
    Pop,
    Transition
};

struct LayerAction
{
    LayerActionType Type;
    std::type_index FromType = std::type_index(typeid(void));
    std::unique_ptr<Layer> ToLayer = nullptr;
};

class Application
{
  private:
    ApplicationSpecification m_Specification;
    bool m_Running = false;

    std::vector<std::unique_ptr<Layer>> m_LayerStack;

    static std::queue<LayerAction> s_PendingActions;

    void ProcessPendingActions();

  public:
    Application(const ApplicationSpecification &specification = ApplicationSpecification());
    ~Application();

    static std::shared_ptr<TexturesManager> GetTexturesManager();
    static std::shared_ptr<InputManager> GetInputManager();

    void Run();
    void Stop();

    static void QueuePush(std::unique_ptr<Layer> layer);

    static void QueuePop(std::type_index layerType);

    template <typename TLayer> static void QueuePop()
    {
        static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
        QueuePop(std::type_index(typeid(TLayer)));
    }

    static void QueueTransition(std::type_index fromType, std::unique_ptr<Layer> toLayer);

    static Application &Get();

    template <typename TLayer> TLayer *GetLayer()
    {
        static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
        for (const auto &layer : m_LayerStack)
        {
            if (typeid(*layer) == typeid(TLayer))
            {
                return static_cast<TLayer *>(layer.get());
            }
        }
        return nullptr;
    }
};

// Template implementations for Layer transition methods
template <typename TLayer> void Layer::TransitionTo(std::unique_ptr<TLayer> layer)
{
    static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
    Application::QueueTransition(GetTypeIndex(), std::move(layer));
}

template <typename TLayer> void Layer::Push(std::unique_ptr<TLayer> layer)
{
    static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
    Application::QueuePush(std::move(layer));
}

} // namespace Core