#include "engine/core/layer.h"
#include "engine/core/application.h"

namespace engine
{

void Layer::SuspendUpdate()
{
    m_state = State::SUSPENDED_UPDATE;
}

void Layer::SuspendUpdateAndRender()
{
    m_state = State::SUSPENDED_UPDATE_AND_RENDER;
}

void Layer::Resume()
{
    m_state = State::ACTIVE;
}

Layer::State Layer::GetState() const
{
    return m_state;
}

void Layer::Pop()
{
    Application::QueuePop(GetTypeIndex());
}

} // namespace engine
