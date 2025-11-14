#pragma once

#include "Event.h"
#include <memory>
#include <typeindex>

namespace Core
{
class Layer
{
  public:
    enum class State
    {
        Active,
        Suspended_Update,
        Suspended_UpdateAndRender,
    };

  public:
    virtual ~Layer() = default;

    virtual void OnEvent(Event &event)
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
    template <typename TLayer> void TransitionTo(std::unique_ptr<TLayer> layer);

    template <typename TLayer> void Push(std::unique_ptr<TLayer> layer);

    void Pop();

    std::type_index GetTypeIndex() const
    {
        return std::type_index(typeid(*this));
    }

  protected:
    State m_State = State::Active;
};

} // namespace Core