#pragma once

#include "engine/core/event.h"
#include <memory>
#include <typeindex>

namespace engine
{
class Layer
{
  public:
    enum class State
    {
        ACTIVE,
        SUSPENDED_UPDATE,
        SUSPENDED_UPDATE_AND_RENDER,
    };

  protected:
    State m_state = State::ACTIVE;

  public:
    virtual ~Layer() = default;

    virtual void OnEvent(Event& event)
    {
    }

    virtual void OnUpdate(float ts)
    {
    }

    virtual void OnRender()
    {
    }

    void SuspendUpdate();
    void SuspendUpdateAndRender();
    void Resume();

    State GetState() const;

    // Layer transition methods defined in Application.h
    template <typename TLayer> void TransistionTo(std::unique_ptr<TLayer> layer);

    template <typename TLayer> void Push(std::unique_ptr<TLayer> layer);

    void Pop();

    std::type_index GetTypeIndex() const
    {
        return std::type_index(typeid(*this));
    }
};

} // namespace engine