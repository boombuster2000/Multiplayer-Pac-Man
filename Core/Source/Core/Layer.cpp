#include "Layer.h"
#include "Application.h"

namespace Core
{

void Layer::SuspendUpdate()
{
    m_State = State::Suspended_Update;
}

void Layer::SuspendUpdateAndRender()
{
    m_State = State::Suspended_UpdateAndRender;
}

void Layer::Resume()
{
    m_State = State::Active;
}

Layer::State Layer::GetState() const
{
    return m_State;
}

void Layer::Pop()
{
    Application::QueuePop(GetTypeIndex());
}

} // namespace Core
