#pragma once

#include "engine/core/input_manager.h"
#include "engine/core/layer.h"
#include "engine/core/textures_manager.h"
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <typeindex>
#include <vector>

namespace engine
{
class InputManager;

struct ApplicationSpecification
{
    std::string name = "Raylib Application";

    uint32_t width = 1280;
    uint32_t height = 720;
    int targetFPS = 60;

    KeyboardKey programExitKey = KEY_ESCAPE;
};

enum class LayerActionType
{
    PUSH,
    POP,
    TRANSITION
};

struct LayerAction
{
    LayerActionType type;
    std::type_index fromType = std::type_index(typeid(void));
    std::unique_ptr<Layer> toLayer = nullptr;
};

class Application
{
  private:
    ApplicationSpecification m_specification;
    bool m_running = false;

    static Application* s_application;
    static std::queue<LayerAction> s_pendingActions;

    TexturesManager m_texturesManager;
    InputManager m_inputManager;
    std::vector<std::unique_ptr<Layer>> m_layerStack;

    void ProcessPendingActions();

  public:
    explicit Application(const ApplicationSpecification& specification = ApplicationSpecification());
    ~Application();

    void Run();
    void Stop();

    static void QueuePush(std::unique_ptr<Layer> layer);

    static void QueuePop(std::type_index layerType);

    template <typename TLayer>
    static void QueuePop()
    {
        static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
        QueuePop(std::type_index(typeid(TLayer)));
    }

    static void QueueTransition(std::type_index fromType, std::unique_ptr<Layer> toLayer);

    static Application& Get();

    static InputManager& GetInputManager();

    static TexturesManager& GetTexturesManager();

    template <typename TLayer>
    TLayer* GetLayer()
    {
        static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
        for (const auto& layer : m_layerStack)
        {
            if (typeid(*layer.get()) == typeid(TLayer))
            {
                return static_cast<TLayer*>(layer.get());
            }
        }
        return nullptr;
    }
};

// Template implementations for Layer transition methods
template <typename TLayer>
void Layer::TransistionTo(std::unique_ptr<TLayer> layer) const
{
    static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
    Application::QueueTransition(GetTypeIndex(), std::move(layer));
}

template <typename TLayer>
void Layer::Push(std::unique_ptr<TLayer> layer) const
{
    static_assert(std::is_base_of_v<Layer, TLayer>, "TLayer must derive from Layer");
    Application::QueuePush(std::move(layer));
}

} // namespace engine